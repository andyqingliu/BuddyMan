// Fill out your copyright notice in the Description page of Project Settings.


#include "BM_Character_Enemy.h"


// Sets default values
ABM_Character_Enemy::ABM_Character_Enemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABM_Character_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABM_Character_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABM_Character_Enemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

