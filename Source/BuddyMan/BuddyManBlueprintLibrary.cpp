// Fill out your copyright notice in the Description page of Project Settings.


#include "BuddyManBlueprintLibrary.h"

FPlayerInfo UBuddyManBlueprintLibrary::GetPlayerInfoByID(int32 PlayerID)
{
	FPlayerInfo PlayerInfo;
	if(PlayerID > 0 && UBM_Common::PlayerInfoMap.Contains(PlayerID))
	{
		PlayerInfo = UBM_Common::PlayerInfoMap[PlayerID];
	}
	return PlayerInfo;
}

UAnimBlueprintGeneratedClass* UBuddyManBlueprintLibrary::GetAnimBlueprintGeneratedClass(UAnimBlueprint* AnimBlueprint)
{
	if(AnimBlueprint)
	{
		return AnimBlueprint->GetAnimBlueprintGeneratedClass();
	}
	return nullptr;
}
