// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BM_AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "BM_AbilityTask_PlayMontageAndWaitForEvent.generated.h"

/** Delegate type used, EventTag and Payload may be empty if it came from the montage callbacks */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBM_PlayMontageAndWaitForEventDelegate, FGameplayTag, EventTag, FGameplayEventData, EventData);

UCLASS()
class UBM_AbilityTask_PlayMontageAndWaitForEvent : public UAbilityTask
{
	GENERATED_BODY()
public:
	UBM_AbilityTask_PlayMontageAndWaitForEvent(const FObjectInitializer& ObjectInitializer);

	virtual void Activate() override;
	virtual void ExternalCancel() override;
	virtual FString GetDebugString() const override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

	/** The montage completely finished playing */
	UPROPERTY(BlueprintAssignable)
	FBM_PlayMontageAndWaitForEventDelegate OnCompleted;

	/** The montage started blending out */
	UPROPERTY(BlueprintAssignable)
	FBM_PlayMontageAndWaitForEventDelegate OnBlendOut;

	/** The montage was interrupted */
	UPROPERTY(BlueprintAssignable)
	FBM_PlayMontageAndWaitForEventDelegate OnInterrupted;

	/** The ability task was explicitly cancelled by another ability */
	UPROPERTY(BlueprintAssignable)
	FBM_PlayMontageAndWaitForEventDelegate OnCancelled;

	/** One of the triggering gameplay events happened */
	UPROPERTY(BlueprintAssignable)
	FBM_PlayMontageAndWaitForEventDelegate EventReceived;

	/**
	 * Play a montage and wait for it end. If a gameplay event happens that matches EventTags (or EventTags is empty), the EventReceived delegate will fire with a tag and event data.
	 * If StopWhenAbilityEnds is true, this montage will be aborted if the ability ends normally. It is always stopped when the ability is explicitly cancelled.
	 * On normal execution, OnBlendOut is called when the montage is blending out, and OnCompleted when it is completely done playing
	 * OnInterrupted is called if another montage overwrites this, and OnCancelled is called if the ability or task is cancelled
	 *
	 * @param TaskInstanceName Set to override the name of this task, for later querying
	 * @param MontageToPlay The montage to play on the character
	 * @param EventTags Any gameplay events matching this tag will activate the EventReceived callback. If empty, all events will trigger callback
	 * @param Rate Change to play the montage faster or slower
	 * @param bStopWhenAbilityEnds If true, this montage will be aborted if the ability ends normally. It is always stopped when the ability is explicitly cancelled
	 * @param AnimRootMotionTranslationScale Change to modify size of root motion or set to 0 to block it entirely
	 */
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UBM_AbilityTask_PlayMontageAndWaitForEvent* PlayMontageAndWaitForEvent(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName,
		UAnimMontage* MontageToPlay,
		FGameplayTagContainer EventTags,
		float Rate = 1.f,
		FName StartSection = NAME_None,
		bool bStopWhenAbilityEnds = true,
		float AnimRootMotionTranslationScale = 1.f);

private:
	UPROPERTY()
	UAnimMontage* MontageToPlay;

	UPROPERTY()
	FGameplayTagContainer EventTags;

	UPROPERTY()
	float Rate;

	UPROPERTY()
	FName StartSection;

	UPROPERTY()
	float AnimRootMotionTranslationScale;

	UPROPERTY()
	bool bStopWhenAbilityEnds;

	bool StopPlayingMontage();

	UBM_AbilitySystemComponent* GetTargetASC();

	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);
	void OnAbilityCancelled();
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload);

	FOnMontageBlendingOutStarted BlendingOutDelegate;
	FOnMontageEnded MontageEndedDelegate;
	FDelegateHandle CancelledHandle;
	FDelegateHandle EventHandle;
};
