// Fill out your copyright notice in the Description page of Project Settings.


#include "BM_Character_Player.h"


// Sets default values
ABM_Character_Player::ABM_Character_Player()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABM_Character_Player::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABM_Character_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABM_Character_Player::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

