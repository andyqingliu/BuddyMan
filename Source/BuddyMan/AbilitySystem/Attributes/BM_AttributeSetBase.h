#pragma once
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BM_AttributeSetBase.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS(BlueprintType)
class BUDDYMAN_API UBM_AttributeSetBase : public  UAttributeSet
{
	GENERATED_BODY()

public:

	UBM_AttributeSetBase();

	ATTRIBUTE_ACCESSORS(UBM_AttributeSetBase, MaxHealth);
	ATTRIBUTE_ACCESSORS(UBM_AttributeSetBase, Health);

public:
	UPROPERTY(BlueprintReadOnly,  ReplicatedUsing=OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	
	UPROPERTY(BlueprintReadOnly,  ReplicatedUsing=OnRep_Health)
	FGameplayAttributeData Health;
	

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
};
