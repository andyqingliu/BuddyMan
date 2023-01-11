// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BM_Common.generated.h"

class UBM_GameplayAbilityBase;

const FString DT_Ability_Path = "DataTable '/Game/BuddyMan/DataTable/DT_CharacterAbility.DT_CharacterAbility'";

UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	Ability1,
	Ability2,
	Ability3,
	Ability4,
	Ability5,
	Ability6,
	Ability7,
	Ability8,
	Ability9,
};

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	None = 0,
	Ghoul = 1,
	Zombie = 2,
	Max = 3
};

USTRUCT(BlueprintType)
struct BUDDYMAN_API FPlayerInfo
{
	GENERATED_BODY()
	FPlayerInfo()
	{
		ID = 0;
		PlayerName = TEXT("");
		PlayerType = 0;
		PlayerController = nullptr;
		HalfHeight = 0;
		RelativeLocation = FVector::Zero();
	}
	
	FPlayerInfo(int32 PlayerID, AController* TmpPlayerController) : ID(PlayerID), PlayerController(TmpPlayerController)
	{
		PlayerName = TEXT("");
		PlayerType = 0;
		HalfHeight = 0;
		RelativeLocation = FVector::Zero();
	}

public:
	UPROPERTY(BlueprintReadWrite)
	int32 ID;

	UPROPERTY(BlueprintReadWrite)
	FString PlayerName;

	UPROPERTY(BlueprintReadWrite)
	int32 PlayerType;

	UPROPERTY(BlueprintReadWrite)
	float HalfHeight;
	
	UPROPERTY(BlueprintReadWrite)
	FVector RelativeLocation;

	UPROPERTY(BlueprintReadWrite)
	AController* PlayerController;
};

USTRUCT()
struct FCharacterAbilityRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 ID;

	UPROPERTY(EditAnywhere)
	ECharacterType Type;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UBM_GameplayAbilityBase>> Abilities;
};

/**
 * 
 */
UCLASS()
class BUDDYMAN_API UBM_Common : public  UObject
{
	GENERATED_BODY()
	
public:
	UBM_Common();

	static inline  int32 StartID = 0;
	static inline TMap<int32, FPlayerInfo> PlayerInfoMap;
	static UDataTable* ReadDataTable(FString Path);
	static FCharacterAbilityRow* GetAbilityRowFromDataTable(ECharacterType CharType);
};
