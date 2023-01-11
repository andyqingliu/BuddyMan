// Fill out your copyright notice in the Description page of Project Settings.


#include "BM_AbilityTask_PlayMontageAndWaitForEvent.h"
#include "GameFramework/Character.h"
#include "AbilitySystemGlobals.h"

UBM_AbilityTask_PlayMontageAndWaitForEvent::UBM_AbilityTask_PlayMontageAndWaitForEvent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Rate = 1.f;
	bStopWhenAbilityEnds = true;
}

void UBM_AbilityTask_PlayMontageAndWaitForEvent::Activate()
{
	if (Ability == nullptr)
	{
		return;
	}

	bool bPlayedMontage = false;
	UBM_AbilitySystemComponent* BM_AbilitySystemComponent = GetTargetASC();

	if (BM_AbilitySystemComponent)
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			// Bind to event callback
			EventHandle = BM_AbilitySystemComponent->AddGameplayEventTagContainerDelegate(EventTags, FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UBM_AbilityTask_PlayMontageAndWaitForEvent::OnGameplayEvent));

			if (BM_AbilitySystemComponent->PlayMontage(Ability, Ability->GetCurrentActivationInfo(), MontageToPlay, Rate, StartSection) > 0.f)
			{
				// Playing a montage could potentially fire off a callback into game code which could kill this ability! Early out if we are  pending kill.
				if (ShouldBroadcastAbilityTaskDelegates() == false)
				{
					return;
				}

				CancelledHandle = Ability->OnGameplayAbilityCancelled.AddUObject(this, &UBM_AbilityTask_PlayMontageAndWaitForEvent::OnAbilityCancelled);

				BlendingOutDelegate.BindUObject(this, &UBM_AbilityTask_PlayMontageAndWaitForEvent::OnMontageBlendingOut);
				AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);

				MontageEndedDelegate.BindUObject(this, &UBM_AbilityTask_PlayMontageAndWaitForEvent::OnMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);

				ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
				if (Character && (Character->GetLocalRole() == ROLE_Authority ||
								  (Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
				{
					Character->SetAnimRootMotionTranslationScale(AnimRootMotionTranslationScale);
				}

				bPlayedMontage = true;
			}
		}
		else
		{
			ABILITY_LOG(Warning, TEXT("URPGAbilityTask_PlayMontageAndWaitForEvent call to PlayMontage failed!"));
		}
	}
	else
	{
		ABILITY_LOG(Warning, TEXT("URPGAbilityTask_PlayMontageAndWaitForEvent called on invalid AbilitySystemComponent"));
	}

	if (!bPlayedMontage)
	{
		ABILITY_LOG(Warning, TEXT("URPGAbilityTask_PlayMontageAndWaitForEvent called in Ability %s failed to play montage %s; Task Instance Name %s."), *Ability->GetName(), *GetNameSafe(MontageToPlay),*InstanceName.ToString());
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}

	SetWaitingOnAvatar();
}

void UBM_AbilityTask_PlayMontageAndWaitForEvent::ExternalCancel()
{
	check(AbilitySystemComponent);

	OnAbilityCancelled();

	Super::ExternalCancel();
}

FString UBM_AbilityTask_PlayMontageAndWaitForEvent::GetDebugString() const
{
	UAnimMontage* PlayingMontage = nullptr;
	if(Ability)
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
		if(AnimInstance != nullptr)
		{
			PlayingMontage = AnimInstance->Montage_IsActive(MontageToPlay) ? MontageToPlay : AnimInstance->GetCurrentActiveMontage();
		}
	}
	return FString::Printf(TEXT("PlayMontageAndWaitForEvent. MontageToPlay: %s (Currently Playing): %s"), *GetNameSafe(MontageToPlay), *GetNameSafe(PlayingMontage));
}

void UBM_AbilityTask_PlayMontageAndWaitForEvent::OnDestroy(bool bAbilityEnded)
{
	// Note: Clearing montage end delegate isn't necessary since its not a multicast and will be cleared when the next montage plays.
	// (If we are destroyed, it will detect this and not do anything)

	// This delegate, however, should be cleared as it is a multicast
	if (Ability)
	{
		Ability->OnGameplayAbilityCancelled.Remove(CancelledHandle);
		if (bAbilityEnded && bStopWhenAbilityEnds)
		{
			StopPlayingMontage();
		}
	}

	UBM_AbilitySystemComponent* RPGAbilitySystemComponent = GetTargetASC();
	if (RPGAbilitySystemComponent)
	{
		RPGAbilitySystemComponent->RemoveGameplayEventTagContainerDelegate(EventTags, EventHandle);
	}

	Super::OnDestroy(bAbilityEnded);
}

UBM_AbilitySystemComponent* UBM_AbilityTask_PlayMontageAndWaitForEvent::GetTargetASC()
{
	return Cast<UBM_AbilitySystemComponent>(AbilitySystemComponent);
}

void UBM_AbilityTask_PlayMontageAndWaitForEvent::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (Ability && Ability->GetCurrentMontage() == MontageToPlay)
	{
		if (Montage == MontageToPlay)
		{
			AbilitySystemComponent->ClearAnimatingAbility(Ability);

			// Reset AnimRootMotionTranslationScale
			ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
			if (Character && (Character->GetLocalRole() == ROLE_Authority ||
							  (Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
			{
				Character->SetAnimRootMotionTranslationScale(1.f);
			}

		}
	}

	if (bInterrupted)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnInterrupted.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
	else
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnBlendOut.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
}

void UBM_AbilityTask_PlayMontageAndWaitForEvent::OnAbilityCancelled()
{
	// TODO: Merge this fix back to engine, it was calling the wrong callback

	if (StopPlayingMontage())
	{
		// Let the BP handle the interrupt as well
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
}

void UBM_AbilityTask_PlayMontageAndWaitForEvent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCompleted.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}

	EndTask();
}

void UBM_AbilityTask_PlayMontageAndWaitForEvent::OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		FGameplayEventData TempData = *Payload;
		TempData.EventTag = EventTag;

		EventReceived.Broadcast(EventTag, TempData);
	}
}

UBM_AbilityTask_PlayMontageAndWaitForEvent* UBM_AbilityTask_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(UGameplayAbility* OwningAbility,
	FName TaskInstanceName, UAnimMontage* MontageToPlay, FGameplayTagContainer EventTags, float Rate, FName StartSection, bool bStopWhenAbilityEnds, float AnimRootMotionTranslationScale)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(Rate);

	UBM_AbilityTask_PlayMontageAndWaitForEvent* MyObj = NewAbilityTask<UBM_AbilityTask_PlayMontageAndWaitForEvent>(OwningAbility, TaskInstanceName);
	MyObj->MontageToPlay = MontageToPlay;
	MyObj->EventTags = EventTags;
	MyObj->Rate = Rate;
	MyObj->StartSection = StartSection;
	MyObj->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;
	MyObj->bStopWhenAbilityEnds = bStopWhenAbilityEnds;

	
	return MyObj;
}

bool UBM_AbilityTask_PlayMontageAndWaitForEvent::StopPlayingMontage()
{
	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	if (!ActorInfo)
	{
		return false;
	}

	UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return false;
	}

	// Check if the montage is still playing
	// The ability would have been interrupted, in which case we should automatically stop the montage
	if (AbilitySystemComponent && Ability)
	{
		if (AbilitySystemComponent->GetAnimatingAbility() == Ability
			&& AbilitySystemComponent->GetCurrentMontage() == MontageToPlay)
		{
			// Unbind delegates so they don't get called as well
			FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay);
			if (MontageInstance)
			{
				MontageInstance->OnMontageBlendingOutStarted.Unbind();
				MontageInstance->OnMontageEnded.Unbind();
			}

			AbilitySystemComponent->CurrentMontageStop();
			return true;
		}
	}

	return false;
}




