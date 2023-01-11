// Fill out your copyright notice in the Description page of Project Settings.


#include "BM_GameStateBase.h"

#include "GameFramework/PlayerState.h"


ABM_GameStateBase::ABM_GameStateBase()
{
}

void ABM_GameStateBase::BeginPlay()
{
	UE_LOG(LogTemp, Log, TEXT("ABM_GameStateBase BeginPlay ...."));
}

void ABM_GameStateBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	
}

void ABM_GameStateBase::AddPlayerState(APlayerState* PlayerState)
{
	UE_LOG(LogTemp, Log, TEXT("ABM_GameStateBase AddPlayerState ....PlayerState PlayerID=%d"), PlayerState->GetPlayerId());
}


