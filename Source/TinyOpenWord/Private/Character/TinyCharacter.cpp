// Copyright INVI1998, Inc. All Rights Reserved.


#include "Character/TinyCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


ATinyCharacter::ATinyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;	// 不使用控制器旋转俯仰
	bUseControllerRotationYaw = false;		// 不使用控制器旋转偏航
	bUseControllerRotationRoll = false;	// 不使用控制器旋转滚动

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bEnableCameraLag = true;	// 启用相机延迟
	CameraBoom->CameraLagSpeed = 15.0f;		// 相机延迟速度
	CameraBoom->bUsePawnControlRotation = true;	// 使用控制器旋转

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;	// 不使用控制器旋转

	GetCharacterMovement()->bOrientRotationToMovement = true;	// 朝向移动方向
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);	// 旋转速度，只影响朝向移动方向
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



