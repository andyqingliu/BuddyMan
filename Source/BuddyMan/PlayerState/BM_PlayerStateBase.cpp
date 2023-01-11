#include "BM_PlayerStateBase.h"

#include "BuddyMan/Character/BM_CharacterBase.h"
#include "Net/UnrealNetwork.h"

ABM_PlayerStateBase::ABM_PlayerStateBase()
{
}

void ABM_PlayerStateBase::OnRep_PlayerInfo()
{
	UE_LOG(LogTemp, Log, TEXT("ABM_PlayerStateBase OnRep_PlayerInfo"));
}

void ABM_PlayerStateBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABM_PlayerStateBase, HalfHeight);
	DOREPLIFETIME(ABM_PlayerStateBase, RelativeLocation);
	DOREPLIFETIME(ABM_PlayerStateBase, PlayerInfo);
}

void ABM_PlayerStateBase::OnRep_HalfHeight()
{
	
}

void ABM_PlayerStateBase::OnRep_RelativeLocation()
{
	
}



