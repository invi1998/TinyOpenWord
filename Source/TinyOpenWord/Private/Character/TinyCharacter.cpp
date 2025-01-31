// Copyright INVI1998, Inc. All Rights Reserved.


#include "Character/TinyCharacter.h"


ATinyCharacter::ATinyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATinyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATinyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATinyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}



