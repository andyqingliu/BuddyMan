// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BM_CharacterBase.h"
#include "BM_TargetType.generated.h"

UCLASS(Blueprintable, meta=(ShowWorldContextPin))
class BUDDYMAN_API UBM_TargetType : public UObject
{
	GENERATED_BODY()

public:
	UBM_TargetType() {}

	UFUNCTION(BlueprintNativeEvent)
	void GetTargets(ABM_CharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;
};

UCLASS(NotBlueprintable)
class BUDDYMAN_API UBM_TargetType_UseOwner : public UBM_TargetType
{
	GENERATED_BODY()
	
public:
	UBM_TargetType_UseOwner() {}

	virtual void GetTargets_Implementation(ABM_CharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};

UCLASS(NotBlueprintable)
class BUDDYMAN_API UBM_TargetType_UseEventData : public UBM_TargetType
{
	GENERATED_BODY()
	
public:
	UBM_TargetType_UseEventData() {}

	virtual void GetTargets_Implementation(ABM_CharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};