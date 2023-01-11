#include "UBM_GameplayAbilityBase.h"

#include "AbilitySystemComponent.h"

UBM_GameplayAbilityBase::UBM_GameplayAbilityBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
}

void UBM_GameplayAbilityBase::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
	if(bAutoActivate)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}

FBM_GameplayEffectContainerSpec UBM_GameplayAbilityBase::MakeEffectContainerSpecFromContainer(const FBM_GameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	FBM_GameplayEffectContainerSpec Spec;
	AActor* OwningActor = GetOwningActorFromActorInfo();
	ABM_CharacterBase* OwningCharacter = Cast<ABM_CharacterBase>(OwningActor);
	UBM_AbilitySystemComponent* OwningASC = UBM_AbilitySystemComponent::GetAbilitySystemComponentFrmoActor(OwningActor);
	if(OwningASC)
	{
		if(Container.TargetType.Get())
		{
			TArray<FHitResult> HitResults;
			TArray<AActor*> TargetActors;
			const UBM_TargetType* TargetTypeCDO = Container.TargetType.GetDefaultObject();
			AActor* AvatarActor = GetAvatarActorFromActorInfo();
			TargetTypeCDO->GetTargets(OwningCharacter, AvatarActor, EventData, HitResults, TargetActors);
			Spec.AddTargets(HitResults, TargetActors);
		}

		if(OverrideGameplayLevel == INDEX_NONE)
		{
			OverrideGameplayLevel = this->GetAbilityLevel();
		}

		for(const TSubclassOf<UGameplayEffect>& EffectClass : Container.TargetGameplayEffectClasses)
		{
			Spec.TargetGameplayEffectSpecs.Add(MakeOutgoingGameplayEffectSpec(EffectClass, OverrideGameplayLevel));
		}
	}
	return Spec;
}



FBM_GameplayEffectContainerSpec UBM_GameplayAbilityBase::MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	FBM_GameplayEffectContainer* FoundContainer = EffectContainerMap.Find(ContainerTag);
	if(FoundContainer)
	{
		return MakeEffectContainerSpecFromContainer(*FoundContainer, EventData, OverrideGameplayLevel);
	}
	return FBM_GameplayEffectContainerSpec();
}


TArray<FActiveGameplayEffectHandle> UBM_GameplayAbilityBase::ApplyEffectContainerSpec(const FBM_GameplayEffectContainerSpec& ContainerSpec)
{
	TArray<FActiveGameplayEffectHandle> AllEffects;

	for (const FGameplayEffectSpecHandle& SpecHandle : ContainerSpec.TargetGameplayEffectSpecs)
	{
		AllEffects.Append(K2_ApplyGameplayEffectSpecToTarget(SpecHandle, ContainerSpec.TargetData));
	}
	return AllEffects;
}


TArray<FActiveGameplayEffectHandle> UBM_GameplayAbilityBase::ApplyEffectContainer(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	FBM_GameplayEffectContainerSpec Spec = MakeEffectContainerSpec(ContainerTag, EventData, OverrideGameplayLevel);
	//TODO:: 找出目标actor
	return ApplyEffectContainerSpec(Spec);
}

