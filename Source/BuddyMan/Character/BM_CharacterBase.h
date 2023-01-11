// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "BuddyMan/AbilitySystem/BM_AbilitySystemComponent.h"
#include "BuddyMan/AbilitySystem/Attributes/BM_AttributeSetBase.h"
#include "GameFramework/Character.h"
#include "PlayerState/BM_PlayerStateBase.h"
#include "BM_CharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBM_CharacterInfoChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBM_CharacterHpChanged);

struct FOnAttributeChangeData;
class UBM_GameplayAbilityBase;

UCLASS()
class BUDDYMAN_API ABM_CharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABM_CharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void InitAttributes();

	virtual void AddCharacterStartupAbilities();

	virtual void AddStartupEffects();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	float GetCharacterLevel();
	int32 GetAbilityLevelByID(int32 AbilityID);
	void BindASCInput();
	void InitCharacterAbilities();

private:
	UBM_AbilitySystemComponent* AbilitySystemComponent;
	bool bHasBindInput;

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<USkeletalMesh*> SkeletalMeshes;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UAnimBlueprint*> AnimClasses;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<float> CharacterHalfHeight;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<TSubclassOf<UGameplayEffect>> StartupEffects;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UBM_AttributeSetBase* AttributeSet;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CurMaxHp)
	float CurMaxHp;
	
	UFUNCTION()
	void OnRep_CurMaxHp();

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CurHp)
	float CurHp;
	
	UFUNCTION()
	void OnRep_CurHp();

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CurPlayerState)
	ABM_PlayerStateBase* CurPlayerState;
	
	UFUNCTION()
	void OnRep_CurPlayerState();

	UFUNCTION(Server, Reliable)
	void SendGameplayEventToSelf(FGameplayTag EventTag, FGameplayEventData Payload);

	UPROPERTY(BlueprintAssignable)
	FBM_CharacterHpChanged OnHpChanged;

	UPROPERTY(BlueprintAssignable)
	FBM_CharacterInfoChanged OnCharacterInfoChange;

	UFUNCTION(BlueprintCallable)
	void InitMesh();

	UFUNCTION(BlueprintCallable)
	void InitHeight();

public:
	int32 GetRandomCharacter();
	void SetHalfHeight();
	void SetRelativeLocation();
	
public:
	UPROPERTY(BlueprintReadWrite)
	ECharacterType CharacterType = ECharacterType::None;

private:
	TArray<TSubclassOf<UBM_GameplayAbilityBase>> CharAbilities;
};
