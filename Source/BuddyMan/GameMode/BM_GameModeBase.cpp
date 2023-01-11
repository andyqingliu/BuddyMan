// Fill out your copyright notice in the Description page of Project Settings.


#include "BM_GameModeBase.h"

#include "BuddyMan/Character/BM_CharacterBase.h"
#include "Character/BM_Character_Enemy.h"
#include "Character/BM_Character_Player.h"
#include "Common/BM_Common.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerState/BM_PlayerStateBase.h"


// Sets default values
ABM_GameModeBase::ABM_GameModeBase()
{
}

void ABM_GameModeBase::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("ABM_GameModeBase BeginPlay ...."));
	
	// if(CurController)
	// {
	// 	AActor* TmpActor = GetWorld()->SpawnActor(SpawnClass);
	// 	CurController->Possess(Cast<APawn>(TmpActor));
	// 	UE_LOG(LogTemp, Log, TEXT("ABM_GameModeBase SpawnActor ...."));
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Log, TEXT("ABM_GameModeBase SpawnActor failed ...."));
	// }
}

void ABM_GameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	CurController = NewPlayer;
	UBM_Common::StartID += 1;
	
	ABM_PlayerStateBase* CurPlayerState = Cast<ABM_PlayerStateBase>(NewPlayer->PlayerState);
	CurPlayerState->SetPlayerId(UBM_Common::StartID);
	const FString Name = FString::Printf(TEXT("Player_%d"), UBM_Common::StartID);
	CurPlayerState->SetPlayerName(Name);
	int32 Rnd = RndCharacterType();
	while(TmpRnd == Rnd)
	{
		Rnd = RndCharacterType();
		if(TmpRnd != Rnd)
		{
			break;
		}
	}
	TmpRnd = Rnd;
	float TmpHalfHeight = 0.f;
	if(HalfHeights.Num() > (Rnd - 1))
	{
		TmpHalfHeight = HalfHeights[Rnd - 1];
	}
	CurPlayerState->HalfHeight = TmpHalfHeight;
	const FVector TmpRelativeVec = FVector(0, 0, -TmpHalfHeight);
	CurPlayerState->RelativeLocation = TmpRelativeVec;

	FPlayerInfo TmpPlayerInfo(UBM_Common::StartID, NewPlayer);
	TmpPlayerInfo.PlayerName = Name;
	TmpPlayerInfo.PlayerType = Rnd;
	TmpPlayerInfo.HalfHeight = TmpHalfHeight;
	TmpPlayerInfo.RelativeLocation = TmpRelativeVec;
	CurPlayerState->PlayerInfo = TmpPlayerInfo;

	//NewPlayer->PlayerState = CurPlayerState;
	
	UE_LOG(LogTemp, Log, TEXT("ABM_GameModeBase OnPostLogin ....PlayerID = %d, PlayerType = %d"), UBM_Common::StartID, Rnd);
}

void ABM_GameModeBase::Logout(AController* Exiting)
{
	UBM_Common::StartID = 0;
}

int32 ABM_GameModeBase::RndCharacterType()
{
	const int32 MaxCharacterType = static_cast<int32>(ECharacterType::Max) - 1;
	const int32 MinCharacterType = static_cast<int32>(ECharacterType::None) + 1;
	const int32 Rnd = UKismetMathLibrary::RandomIntegerInRange(MinCharacterType, MaxCharacterType);
	return Rnd;
}
