#pragma once

#include "CoreMinimal.h"
#include "Common/BM_Common.h"
#include "BuddyManBlueprintLibrary.generated.h"

UCLASS(meta=(ScriptName="BuddyManBlueprintLibrary"))
class BUDDYMAN_API UBuddyManBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static FPlayerInfo GetPlayerInfoByID(int32 PlayerID);

	UFUNCTION(BlueprintCallable)
	static UAnimBlueprintGeneratedClass* GetAnimBlueprintGeneratedClass(UAnimBlueprint* AnimBlueprint);
};