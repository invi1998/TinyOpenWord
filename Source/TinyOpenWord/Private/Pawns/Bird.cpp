// Copyright INVI1998, Inc. All Rights Reserved.


#include "Pawns/Bird.h"


ABird::ABird()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABird::BeginPlay()
{
	Super::BeginPlay();
	
}



