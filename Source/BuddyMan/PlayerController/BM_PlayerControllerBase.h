// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BM_PlayerControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class BUDDYMAN_API ABM_PlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	ABM_PlayerControllerBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void AcknowledgePossession(class APawn* P) override;
	virtual void OnRep_PlayerState() override;
	virtual void SetupInputComponent() override;

	void OnTestMouseButton();
};
