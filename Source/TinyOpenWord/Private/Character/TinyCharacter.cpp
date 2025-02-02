// Copyright INVI1998, Inc. All Rights Reserved.


#include "Character/TinyCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GroomComponent.h"


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

	FaceComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FaceComponent"));
	FaceComponent->SetupAttachment(GetMesh());

	if (FaceComponent->GetSkeletalMeshAsset())
	{
		
	}

	Mustache = CreateDefaultSubobject<UGroomComponent>(TEXT("Mustache"));
	Mustache->SetupAttachment(FaceComponent);
	Mustache->AttachmentName = TEXT("FACIAL_C_LipUpper");

	Eyelashes = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyelashes"));
	Eyelashes->SetupAttachment(FaceComponent);
	Eyelashes->AttachmentName = TEXT("FACIAL_C_FacialRoot");

	Fuzz = CreateDefaultSubobject<UGroomComponent>(TEXT("Fuzz"));
	Fuzz->SetupAttachment(FaceComponent);
	Fuzz->AttachmentName = TEXT("FACIAL_C_FacialRoot");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(FaceComponent);
	Eyebrows->AttachmentName = TEXT("FACIAL_C_FacialRoot");

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(FaceComponent);
	Hair->AttachmentName = TEXT("FACIAL_C_FacialRoot");

	Beard = CreateDefaultSubobject<UGroomComponent>(TEXT("Beard"));
	Beard->SetupAttachment(FaceComponent);
	Beard->AttachmentName = TEXT("Facial_C_JAW");
	
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

void ATinyCharacter::EnableMasterPose(USkeletalMeshComponent* MeshComponent)
{
	if (IsValid(MeshComponent->GetSkeletalMeshAsset()))
	{
		if (IsValid(MeshComponent->GetAnimClass()) || IsValid(MeshComponent->GetAnimInstance()))
		{
			MeshComponent->SetLeaderPoseComponent(GetMesh());
		}
	}
}



