// Fill out your copyright notice in the Description page of Project Settings.


#include "BM_PlayerControllerBase.h"

#include "BuddyMan/Character/BM_CharacterBase.h"
#include "GameFramework/PlayerState.h"
#include "PlayerState/BM_PlayerStateBase.h"

ABM_PlayerControllerBase::ABM_PlayerControllerBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void ABM_PlayerControllerBase::AcknowledgePossession(class APawn* P)
{
	Super::AcknowledgePossession(P);

	ABM_PlayerStateBase* TmpPlayerState = Cast<ABM_PlayerStateBase>(PlayerState);
	if(TmpPlayerState)
	{
		FString TmpPlayerName = TmpPlayerState->GetPlayerName();
		UE_LOG(LogTemp, Log, TEXT("ABM_PlayerControllerBase AcknowledgePossession TmpPlayerName=%s"), *TmpPlayerName);
	}
	ABM_CharacterBase* CharacterBase = Cast<ABM_CharacterBase>(P);
	if(CharacterBase)
	{
		CharacterBase->GetAbilitySystemComponent()->InitAbilityActorInfo(CharacterBase, CharacterBase);
		UE_LOG(LogTemp, Log, TEXT("ABM_PlayerControllerBase AcknowledgePossession ...."));
	}
}

void ABM_PlayerControllerBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	UE_LOG(LogTemp, Log, TEXT("ABM_PlayerControllerBase OnRep_PlayerState ...."));
}

void ABM_PlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("TestLeftMouseButton", EInputEvent::IE_Pressed, this, &ABM_PlayerControllerBase::OnTestMouseButton);
}

void ABM_PlayerControllerBase::OnTestMouseButton()
{
	UE_LOG(LogTemp, Log, TEXT("ABM_PlayerControllerBase OnTestMouseButton ...."));
}