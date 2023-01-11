#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameState.h"
#include "BM_GameStateBase.generated.h"

UCLASS()
class BUDDYMAN_API ABM_GameStateBase : public AGameState
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABM_GameStateBase();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void AddPlayerState(APlayerState* PlayerState) override;
};