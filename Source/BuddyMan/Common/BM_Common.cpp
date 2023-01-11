// Fill out your copyright notice in the Description page of Project Settings.


#include "BM_Common.h"


UBM_Common::UBM_Common()
{
}

UDataTable* UBM_Common::ReadDataTable(FString Path)
{
	UDataTable* DataTable = LoadObject<UDataTable>(nullptr, *Path);
	return DataTable;
}

FCharacterAbilityRow* UBM_Common::GetAbilityRowFromDataTable(ECharacterType CharType)
{
	FCharacterAbilityRow TableRow = FCharacterAbilityRow();
	UDataTable* TmpDataTable = UBM_Common::ReadDataTable(DT_Ability_Path);
	if(TmpDataTable)
	{
		FString ContextString;
		for (auto& name : TmpDataTable->GetRowNames())
		{
			FCharacterAbilityRow* pRow = TmpDataTable->FindRow<FCharacterAbilityRow>(name, ContextString);
			if(pRow)
			{
				UE_LOG(LogTemp, Log, TEXT("UBM_Common GetAbilityRowFromDataTable Success ...."));
				return pRow;
			}
		}
	}
	return &TableRow;
}
