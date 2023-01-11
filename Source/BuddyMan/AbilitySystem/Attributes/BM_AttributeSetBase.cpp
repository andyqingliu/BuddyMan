#include "BM_AttributeSetBase.h"
#include "Net/UnrealNetwork.h"

UBM_AttributeSetBase::UBM_AttributeSetBase()
{
}

void UBM_AttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UBM_AttributeSetBase, Health, COND_OwnerOnly, REPNOTIFY_Always);
}

void UBM_AttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBM_AttributeSetBase, Health, OldHealth)
}

void UBM_AttributeSetBase::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBM_AttributeSetBase, MaxHealth, OldMaxHealth)
}