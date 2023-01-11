#include "BM_AbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "Character/BM_CharacterBase.h"
#include "Common/BM_Common.h"
#include "Kismet/GameplayStatics.h"

UBM_AbilitySystemComponent::UBM_AbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bHasGivenCharacterAbilities = false;
	bHasApplyStartupEffects = false;
}

void UBM_AbilitySystemComponent::AbilityLocalInputPressed(int32 InputID)
{
	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.InputID == InputID)
		{
			bool bAbilityIsActive = Spec.IsActive();
			UE_LOG(LogTemp, Log, TEXT("UBM_AbilitySystemComponent AbilityLocalInputPressed InputID ===== %d bIsActive = %d"), InputID, bAbilityIsActive);
		}
	}
	Super::AbilityLocalInputPressed(InputID);
	UE_LOG(LogTemp, Log, TEXT("UBM_AbilitySystemComponent AbilityLocalInputPressed InputID=%d"), InputID);
	
}

void UBM_AbilitySystemComponent::AbilityLocalInputReleased(int32 InputID)
{
	if(InputID == (int32)EAbilityInputID::Ability2)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		APawn* Pawn = PlayerController->GetPawn();
		ABM_CharacterBase* Character = Cast<ABM_CharacterBase>(Pawn);
		if(Character)
		{
			FGameplayTag Tag = FGameplayTag::RequestGameplayTag("Ability.Attack");
			FGameplayEventData EventData = FGameplayEventData();
			EventData.EventMagnitude = 0;
			Character->SendGameplayEventToSelf(Tag, EventData);
		}
	}
}

UBM_AbilitySystemComponent* UBM_AbilitySystemComponent::GetAbilitySystemComponentFrmoActor(const AActor* Actor, bool LookForComponent)
{
	return Cast<UBM_AbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, LookForComponent));
}
