// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/BM_Common.h"
#include "GameFramework/PlayerState.h"
#include "BM_PlayerStateBase.generated.h"

struct FPlayerInfo;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnReceivePlayerName, FString, Name);

UCLASS()
class BUDDYMAN_API ABM_PlayerStateBase : public APlayerState
{
	GENERATED_BODY()

public:
	ABM_PlayerStateBase();
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_HalfHeight)
	float HalfHeight;
	
	UFUNCTION()
	void OnRep_HalfHeight();

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_RelativeLocation)
	FVector RelativeLocation;
	
	UFUNCTION()
	void OnRep_RelativeLocation();

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PlayerInfo)
	FPlayerInfo PlayerInfo;

	UFUNCTION()
	void OnRep_PlayerInfo();
};