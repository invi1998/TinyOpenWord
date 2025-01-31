// Copyright INVI1998, Inc. All Rights Reserved.


#include "Player/TinyPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

ATinyPlayerController::ATinyPlayerController()
{
}

void ATinyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* InputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (InputLocalPlayerSubsystem)
	{
		InputLocalPlayerSubsystem->AddMappingContext(DefaultIMC, 0);
	}
}

void ATinyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* ShooterInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	if (ShooterInputComponent)
	{
		ShooterInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATinyPlayerController::Input_Look);
		ShooterInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATinyPlayerController::Input_Move);
	}
}

void ATinyPlayerController::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();		// 获取控制器旋转
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);	// 只保留Yaw旋转

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);	// 通过旋转矩阵获取X轴
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);		// 通过旋转矩阵获取Y轴

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void ATinyPlayerController::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2D>();
	AddYawInput(InputAxisVector.X);
	AddPitchInput(InputAxisVector.Y);
}
