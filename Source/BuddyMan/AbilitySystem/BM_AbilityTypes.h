// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BM_TargetType.h"
#include "BM_AbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FBM_GameplayEffectContainer
{
	GENERATED_BODY()

public:
	FBM_GameplayEffectContainer(){}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	TSubclassOf<UBM_TargetType> TargetType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	TArray<TSubclassOf<UGameplayEffect>> TargetGameplayEffectClasses;
};

USTRUCT(BlueprintType)
struct FBM_GameplayEffectContainerSpec
{
	GENERATED_BODY()
	
public:
	FBM_GameplayEffectContainerSpec(){}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	FGameplayAbilityTargetDataHandle TargetData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	TArray<FGameplayEffectSpecHandle> TargetGameplayEffectSpecs;

	bool HasValidEffects() const;

	bool HasValidTargets() const;

	void AddTargets(const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors);
};