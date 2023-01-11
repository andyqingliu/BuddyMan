// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BM_EnemyDetectComponent.generated.h"

UCLASS()
class BUDDYMAN_API UBM_EnemyDetectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBM_EnemyDetectComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	AActor* GetNearestActor();

private:
	TArray<AActor*> EnemyActors;
	 
};