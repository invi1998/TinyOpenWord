// Copyright INVI1998, Inc. All Rights Reserved.


#include "Pawns/Bird.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"


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

	// AutoPossessPlayer = EAutoReceiveInput::Player0;	// 自动接收玩家输入

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 300.f;

	BirdCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("BirdCameraComponent"));
	BirdCameraComponent->SetupAttachment(SpringArmComponent);

	bUseControllerRotationPitch = true;		// 使用控制器旋转俯仰
	bUseControllerRotationYaw = true;		// 使用控制器旋转偏航
}

void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABird::Input_Move);
	    EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABird::Input_Look);
	}
	
}

void ABird::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			InputLocalPlayerSubsystem->AddMappingContext(DefaultIMC, 0);
		}
	}
}

void ABird::Input_Move(const FInputActionValue& InputActionValue)
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("InputAxisVector: %s"), *InputAxisVector.ToString()));
	
	if (GetController())
	{
		const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		AddMovementInput(ForwardDirection, InputAxisVector.Y);
		AddMovementInput(RightDirection, InputAxisVector.X);
	}
	
}

void ABird::Input_Look(const FInputActionValue& InputActionValue)
{
	if (GetController())
	{
		const FVector2d InputAxisVector = InputActionValue.Get<FVector2D>();
		AddControllerYawInput(InputAxisVector.X);
		AddControllerPitchInput(InputAxisVector.Y);
	}
}




