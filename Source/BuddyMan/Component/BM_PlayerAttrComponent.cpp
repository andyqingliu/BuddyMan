// Fill out your copyright notice in the Description page of Project Settings.


#include "BM_PlayerAttrComponent.h"

#include "GameplayEffectExtension.h"
#include "BuddyMan/Character/BM_CharacterBase.h"
#include "BuddyMan/Common/BM_Common.h"
#include "BuddyMan/GameMode/BM_GameModeBase.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UBM_PlayerAttrComponent::UBM_PlayerAttrComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBM_PlayerAttrComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	check(Owner);

	UBM_AbilitySystemComponent* ASC = Cast<UBM_AbilitySystemComponent>(Owner->GetComponentByClass(UBM_AbilitySystemComponent::StaticClass()));
	if(ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UBM_AttributeSetBase::GetMaxHealthAttribute()).AddUObject(this, &UBM_PlayerAttrComponent::HandleMaxHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UBM_AttributeSetBase::GetHealthAttribute()).AddUObject(this, &UBM_PlayerAttrComponent::HandleHealthChanged);
	}
	InitAttributeSet();
}


// Called every frame
void UBM_PlayerAttrComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

static AActor* GetInstigatorFromAttributeChangeData(const FOnAttributeChangeData& ChangeData)
{
	if(ChangeData.GEModData)
	{
		const FGameplayEffectContextHandle& EffectContext = ChangeData.GEModData->EffectSpec.GetEffectContext();
		return EffectContext.GetOriginalInstigator();
	}
	return nullptr;
}

void UBM_PlayerAttrComponent::HandleMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	OnMaxHealthChanged.Broadcast(GetInstigatorFromAttributeChangeData(ChangeData), ChangeData.OldValue, ChangeData.NewValue);
	ABM_CharacterBase* Character = Cast<ABM_CharacterBase>(GetOwner());
	if(Character)
	{
		Character->CurMaxHp = ChangeData.NewValue;
	}
}

void UBM_PlayerAttrComponent::HandleHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	OnHealthChanged.Broadcast(GetInstigatorFromAttributeChangeData(ChangeData), ChangeData.OldValue, ChangeData.NewValue);
	ABM_CharacterBase* Character = Cast<ABM_CharacterBase>(GetOwner());
	if(Character)
	{
		Character->CurHp = ChangeData.NewValue;
	}
}

UBM_AttributeSetBase* UBM_PlayerAttrComponent::GetAttributeSet()
{
	AActor* Owner = GetOwner();
	check(Owner);

	UBM_AbilitySystemComponent* ASC = Cast<UBM_AbilitySystemComponent>(Owner->GetComponentByClass(UBM_AbilitySystemComponent::StaticClass()));
	if(ASC)
	{
		const UBM_AttributeSetBase* PlayerAttributeSet =  ASC->GetSet<UBM_AttributeSetBase>();
		return const_cast<UBM_AttributeSetBase*>(PlayerAttributeSet);
	}
	return nullptr;
}

void UBM_PlayerAttrComponent::InitAttributeSet()
{
	UBM_AttributeSetBase* PlayerAttributeSet = GetAttributeSet();
	if(PlayerAttributeSet)
	{
		//初始化最大血量
		PlayerAttributeSet->InitMaxHealth(100);
		PlayerAttributeSet->InitHealth(100);
	}
	ABM_CharacterBase* Character = Cast<ABM_CharacterBase>(GetOwner());
	if(Character)
	{
		Character->CurMaxHp = 100.f;
		Character->CurHp = 100.f;
	}
}

float UBM_PlayerAttrComponent::GetCurrentMaxHp()
{
	UBM_AttributeSetBase* PlayerAttributeSet = GetAttributeSet();
	if(PlayerAttributeSet)
	{
		return PlayerAttributeSet->GetMaxHealth();
	}
	return 0.f;
}

float UBM_PlayerAttrComponent::GetCurrentHp()
{
	UBM_AttributeSetBase* PlayerAttributeSet = GetAttributeSet();
	if(PlayerAttributeSet)
	{
		return PlayerAttributeSet->GetHealth();
	}
	return 0.f;
}





