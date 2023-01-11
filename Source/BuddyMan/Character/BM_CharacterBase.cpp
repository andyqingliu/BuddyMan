// Fill out your copyright notice in the Description page of Project Settings.


#include "BM_CharacterBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BuddyManBlueprintLibrary.h"
#include "GameplayAbilitySet.h"
#include "AbilitySystem/Abilities/UBM_GameplayAbilityBase.h"
#include "BuddyMan/Common/BM_Common.h"
#include "BuddyMan/PlayerState/BM_PlayerStateBase.h"
#include "Component/BM_PlayerAttrComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ABM_CharacterBase::ABM_CharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bHasBindInput = false;
	AbilitySystemComponent = CreateDefaultSubobject<UBM_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AttributeSet = CreateDefaultSubobject<UBM_AttributeSetBase>(TEXT("AttributeSet"));
	
	UE_LOG(LogTemp, Log, TEXT("ABM_CharacterBase C++ Construct ...."));
}

// Called when the game starts or when spawned
void ABM_CharacterBase::BeginPlay()
{
	UE_LOG(LogTemp, Log, TEXT("ABM_CharacterBase BeginPlay ...."));
	Super::BeginPlay();
}

void ABM_CharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
}

// Called every frame
void ABM_CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABM_CharacterBase::PossessedBy(AController* NewController)
{
	UE_LOG(LogTemp, Log, TEXT("ABM_CharacterBase Start PossessedBy ...."));
	Super::PossessedBy(NewController);

	ABM_PlayerStateBase* TmpPlayerState = Cast<ABM_PlayerStateBase>(NewController->PlayerState);
	if(TmpPlayerState)
	{
		UE_LOG(LogTemp, Log, TEXT("ABM_CharacterBase PossessedBy TmpPlayerState !!!=== NULL ...."));
		CurPlayerState = TmpPlayerState;
		const int32 TmpPlayerID = TmpPlayerState->GetPlayerId();
		const FString TmpPlayerName = TmpPlayerState->GetPlayerName();
		const int32 TmpPlayerType = TmpPlayerState->PlayerInfo.PlayerType;
		const float TmpHalfHeight = TmpPlayerState->PlayerInfo.HalfHeight;
		const FVector TmpRelativeLocation = TmpPlayerState->PlayerInfo.RelativeLocation;
		UE_LOG(LogTemp, Log, TEXT("ABM_CharacterBase BeginPlay CharacterHalfHeight=%f"), TmpHalfHeight);

		CharacterType = static_cast<ECharacterType>(TmpPlayerType);

		InitMesh();
		InitHeight();

		const FString MsgInfo = FString::Printf(TEXT("ABM_CharacterBase BeginPlay PlayerID=%d, Name=%s, CharacterType=%d, HalfHeight=%f, RelativeLocation=%s"), TmpPlayerID, *TmpPlayerName, TmpPlayerType, TmpHalfHeight, *(TmpRelativeLocation.ToString()));
		UKismetSystemLibrary::PrintString(
				this->GetWorld(),
				MsgInfo,
				false, true,
				FLinearColor::Red,
				100
			);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("ABM_CharacterBase PossessedBy TmpPlayerState ===== NULL ...."));
	}

	if(AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
	InitAttributes();
	InitCharacterAbilities();
	AddCharacterStartupAbilities();
	AddStartupEffects();
	
	UE_LOG(LogTemp, Log, TEXT("ABM_CharacterBase End PossessedBy ...."));

	// TObjectPtr<APlayerState> OwnerPlayerState = NewController->PlayerState;
	// if(OwnerPlayerState)
	// {
	// 	UBM_Common::StartID += 1;
	// 	const TObjectPtr<ABM_PlayerStateBase> OwnerState = Cast<ABM_PlayerStateBase>(OwnerPlayerState);
	// 	FString Name = FString::Printf(TEXT("Player_%d"), UBM_Common::StartID);
	// 	PlayerNameNew = Name;
	// 	//OwnerState->SetPlayerName(Name);
	// 	UE_LOG(LogTemp, Log, TEXT("ABM_CharacterBase PossessedBy SetPlayerName=%s"), *Name);
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Log, TEXT("ABM_CharacterBase PossessedBy OwnerPlayerState ==== null"));
	// }
}

void ABM_CharacterBase::InitMesh()
{
	if(CurPlayerState)
	{
		const int32 TmpType = CurPlayerState->PlayerInfo.PlayerType;
		this->GetMesh()->SetSkeletalMesh(SkeletalMeshes[TmpType - 1]);
		this->GetMesh()->SetAnimInstanceClass(AnimClasses[TmpType - 1]->GetAnimBlueprintGeneratedClass());
	}
}

void ABM_CharacterBase::InitHeight()
{
	SetHalfHeight();
	SetRelativeLocation();
}

int32 ABM_CharacterBase::GetRandomCharacter()
{
	const int32 Rnd = UKismetMathLibrary::RandomIntegerInRange(static_cast<int32>(ECharacterType::None) + 1, static_cast<int32>(ECharacterType::Max) - 1);
	return Rnd;
}

void ABM_CharacterBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	BindASCInput();
}

void ABM_CharacterBase::InitAttributes()
{
	if(!AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("InitAttributes failed with no AbilitySystemComponent"));
		return;
	}

	if(!DefaultAttributeEffect)
	{
		UE_LOG(LogTemp, Log, TEXT("InitAttributes failed with no DefaultAttributeEffect"));
		return;
	}

	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, GetCharacterLevel(), EffectContextHandle);
	if(SpecHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), AbilitySystemComponent);
	}
}

void ABM_CharacterBase::InitCharacterAbilities()
{
	if(CurPlayerState)
	{
		int32 TmpType = CurPlayerState->PlayerInfo.PlayerType;
		ECharacterType CharType = static_cast<ECharacterType>(TmpType);
		FCharacterAbilityRow* AbilityRow = UBM_Common::GetAbilityRowFromDataTable(CharType);
		if(AbilityRow->ID > 0)
		{
			CharAbilities.Append(AbilityRow->Abilities);
		}
	}
}

void ABM_CharacterBase::AddCharacterStartupAbilities()
{
	if(GetLocalRole() != ROLE_Authority || !AbilitySystemComponent || AbilitySystemComponent->bHasGivenCharacterAbilities)
	{
		return;
	}

	for(auto ga : CharAbilities)
	{
		FGameplayAbilitySpec spec = FGameplayAbilitySpec(ga, GetAbilityLevelByID(static_cast<int32>(ga.GetDefaultObject()->AbilityInputID)), static_cast<int32>(ga.GetDefaultObject()->AbilityInputID), this);
		AbilitySystemComponent->GiveAbility(spec);
	}
	AbilitySystemComponent->bHasGivenCharacterAbilities = true;
}

void ABM_CharacterBase::AddStartupEffects()
{
	if(GetLocalRole() != ROLE_Authority || !AbilitySystemComponent || AbilitySystemComponent->bHasApplyStartupEffects)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	for(auto ge : StartupEffects)
	{
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(ge, GetCharacterLevel(), EffectContextHandle);
		if(SpecHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), AbilitySystemComponent);
		}
	}
	
	AbilitySystemComponent->bHasApplyStartupEffects = true;
}

UAbilitySystemComponent* ABM_CharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

float ABM_CharacterBase::GetCharacterLevel()
{
	return 1.0f;
}

int32 ABM_CharacterBase::GetAbilityLevelByID(int32 AbilityID)
{
	return 1;
}

void ABM_CharacterBase::BindASCInput()
{
	if(bHasBindInput || !AbilitySystemComponent || !InputComponent)
	{
		return;
	}

	const FGameplayAbilityInputBinds InputBinds = FGameplayAbilityInputBinds(
		TEXT("Confirm"),
		TEXT("Cancel"),
		TEXT("EAbilityInputID"),
		static_cast<int32>(EAbilityInputID::Confirm),
		static_cast<int32>(EAbilityInputID::Cancel)
		);
	AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, InputBinds);
	bHasBindInput = true;
}

void ABM_CharacterBase::SetHalfHeight()
{
	if(CurPlayerState)
	{
		this->GetCapsuleComponent()->SetCapsuleHalfHeight(CurPlayerState->HalfHeight);
		const FString Str = FString::Printf(TEXT("ABM_CharacterBase SetHalfHeight = %f"), CurPlayerState->HalfHeight);
		UKismetSystemLibrary::PrintString(
				this->GetWorld(),
				Str,
				false, true,
				FLinearColor::Red,
				100
			);
	}
}

void ABM_CharacterBase::SetRelativeLocation()
{
	if(CurPlayerState)
	{
		// if(IsLocallyControlled())
		// {
			const FRotator TmpRotator(0, -90, 0);
			const FTransform NewTrans(TmpRotator, CurPlayerState->RelativeLocation, FVector(1, 1, 1));
			this->GetMesh()->SetRelativeTransform(NewTrans);
			const FString Str = FString::Printf(TEXT("ABM_CharacterBase SetRelativeLocation = %s"), *(CurPlayerState->RelativeLocation.ToString()));
			UKismetSystemLibrary::PrintString(
					this->GetWorld(),
					Str,
					false, true,
					FLinearColor::Red,
					100
				);
		// }
		// else
		// {
		// 	BaseTranslationOffset = CurPlayerState->RelativeLocation;
		// }
	}
}

void ABM_CharacterBase::OnRep_CurPlayerState()
{
	UE_LOG(LogTemp, Log, TEXT("OnRep_CurPlayerState .... ID=%d, Name=%s"), CurPlayerState->GetPlayerId(), *(CurPlayerState->GetPlayerName()));
	OnCharacterInfoChange.Broadcast();
}

void ABM_CharacterBase::OnRep_CurMaxHp()
{
	UE_LOG(LogTemp, Log, TEXT("OnRep_CurMaxHp ...."));
	OnHpChanged.Broadcast();
}

void ABM_CharacterBase::OnRep_CurHp()
{
	UE_LOG(LogTemp, Log, TEXT("OnRep_CurHp ...."));
	OnHpChanged.Broadcast();
}

void ABM_CharacterBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABM_CharacterBase, CurPlayerState);
	DOREPLIFETIME(ABM_CharacterBase, CurMaxHp);
	DOREPLIFETIME(ABM_CharacterBase, CurHp);
}

void ABM_CharacterBase::SendGameplayEventToSelf_Implementation(FGameplayTag EventTag, FGameplayEventData Payload)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, EventTag, Payload);
}