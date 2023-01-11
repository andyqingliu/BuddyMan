#pragma once
#include "AbilitySystemComponent.h"
#include "BM_AbilitySystemComponent.generated.h"

UCLASS()
class BUDDYMAN_API UBM_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UBM_AbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void AbilityLocalInputPressed(int32 InputID) override;
	virtual void AbilityLocalInputReleased(int32 InputID) override;

	
public:
	bool bHasGivenCharacterAbilities;
	bool bHasApplyStartupEffects;

	static UBM_AbilitySystemComponent* GetAbilitySystemComponentFrmoActor(const AActor* Actor, bool LookForComponent = false);
};
