// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/BM_AttributeSetBase.h"
#include "BM_PlayerAttrComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FBM_AttributeChanged, AActor*, Instigator, float, OldValue, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBM_AttributeHpChanged);

UCLASS(ClassGroup = "Attr", meta = (BlueprintSpawnableComponent))
class BUDDYMAN_API UBM_PlayerAttrComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBM_PlayerAttrComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UBM_AttributeSetBase* GetAttributeSet();
	void InitAttributeSet();
	
	void HandleMaxHealthChanged(const FOnAttributeChangeData& ChangeData);
	void HandleHealthChanged(const FOnAttributeChangeData& ChangeData);

	UPROPERTY(BlueprintAssignable)
	FBM_AttributeChanged OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FBM_AttributeChanged OnHealthChanged;

	UFUNCTION(BlueprintCallable)
	float GetCurrentHp();

	UFUNCTION(BlueprintCallable)
	float GetCurrentMaxHp();
};