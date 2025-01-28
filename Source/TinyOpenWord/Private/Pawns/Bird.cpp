﻿// Copyright INVI1998, Inc. All Rights Reserved.


#include "Pawns/Bird.h"

#include "Components/CapsuleComponent.h"


ABird::ABird()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->InitCapsuleSize(17.0f, 17.0f);
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RootComponent = CapsuleComponent;

	BirdMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMeshComponent"));
	BirdMeshComponent->SetupAttachment(RootComponent);
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



