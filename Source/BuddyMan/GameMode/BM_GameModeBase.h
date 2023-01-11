// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BM_CharacterBase.h"
#include "GameFramework/Actor.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/GameModeBase.h"
#include "BM_GameModeBase.generated.h"

UCLASS()
class BUDDYMAN_API ABM_GameModeBase : public AGameMode
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABM_GameModeBase();

	UPROPERTY(EditAnywhere)
	TArray<float> HalfHeights;

protected:
	virtual void BeginPlay() override;
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	int32 RndCharacterType();

private:
	int32 CurPlayerID;
	int32 CurPlayerType;
	int32 TmpRnd;
	
	UPROPERTY()
	AController* CurController;
};

