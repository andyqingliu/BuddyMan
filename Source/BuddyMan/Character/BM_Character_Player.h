#pragma once

#include "CoreMinimal.h"
#include "BM_CharacterBase.h"
#include "BM_Character_Player.generated.h"

UCLASS()
class BUDDYMAN_API ABM_Character_Player : public ABM_CharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABM_Character_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};