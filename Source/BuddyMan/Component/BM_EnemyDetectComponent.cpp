// Fill out your copyright notice in the Description page of Project Settings.


#include "BM_EnemyDetectComponent.h"

#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UBM_EnemyDetectComponent::UBM_EnemyDetectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBM_EnemyDetectComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBM_EnemyDetectComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

AActor* UBM_EnemyDetectComponent::GetNearestActor()
{
	if(EnemyActors.Num() > 0)
	{
		TArray<float> TmpDistanceList;
		for (auto Enemy : EnemyActors)
		{
			float TmpDist = UKismetMathLibrary::Vector_Distance(GetOwner()->GetActorLocation(), Enemy->GetActorLocation());
			TmpDistanceList.Add(TmpDist);
		}
		
		int32 Index = 0;
		float Distance = TmpDistanceList[0];
		for(int32 i = 1; i < TmpDistanceList.Num(); i++)
		{
			if(TmpDistanceList[i] < Distance)
			{
				Distance = TmpDistanceList[i];
				Index = i;
			}
		}
		return EnemyActors[Index];
	}
	return nullptr;
}

