// Fill out your copyright notice in the Description page of Project Settings.


#include "BM_TargetType.h"

void UBM_TargetType::GetTargets_Implementation(ABM_CharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	
}

void UBM_TargetType_UseOwner::GetTargets_Implementation(ABM_CharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	OutActors.Add(TargetingCharacter);
}

void UBM_TargetType_UseEventData::GetTargets_Implementation(ABM_CharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	const FHitResult* HitResult = EventData.ContextHandle.GetHitResult();
	if(HitResult)
	{
		OutHitResults.Add(*HitResult);
	}
	else if(EventData.Target)
	{
		OutActors.Add(const_cast<AActor*>(EventData.Target));
	}
}



