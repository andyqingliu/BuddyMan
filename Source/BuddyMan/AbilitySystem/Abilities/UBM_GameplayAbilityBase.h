#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/BM_AbilityTypes.h"
#include "BuddyMan/Common/BM_Common.h"
#include "UBM_GameplayAbilityBase.generated.h"

UCLASS()
class UBM_GameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UBM_GameplayAbilityBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FString AbilityName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EAbilityInputID AbilityInputID = EAbilityInputID::None;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bAutoActivate = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, FBM_GameplayEffectContainer> EffectContainerMap;

	UFUNCTION(BlueprintCallable, Category=Ability, meta=(AutoCreateRefTerm = "EventData"))
	virtual FBM_GameplayEffectContainerSpec MakeEffectContainerSpecFromContainer(const FBM_GameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

	UFUNCTION(BlueprintCallable, Category=Ability, meta=(AutoCreateRefTerm = "EventData"))
	virtual FBM_GameplayEffectContainerSpec MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

	UFUNCTION(BlueprintCallable, Category=Ability, meta=(AutoCreateRefTerm = "EventData"))
	virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainerSpec(const FBM_GameplayEffectContainerSpec& ContainerSpec);

	UFUNCTION(BlueprintCallable, Category=Ability, meta=(AutoCreateRefTerm = "EventData"))
	virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainer(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);
};
