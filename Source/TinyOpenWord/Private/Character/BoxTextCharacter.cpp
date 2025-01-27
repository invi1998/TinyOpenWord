// Copyright INVI1998, Inc. All Rights Reserved.


#include "Character/BoxTextCharacter.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"


// Sets default values
ABoxTextCharacter::ABoxTextCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 创建盒体碰撞框组件
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f)); // 设置盒体大小
	BoxComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName); // 设置碰撞配置
	BoxComponent->SetupAttachment(RootComponent); // 附加到根组件（胶囊碰撞框）
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 初始禁用盒体碰撞
}

// Called when the game starts or when spawned
void ABoxTextCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABoxTextCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABoxTextCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABoxTextCharacter::SwichCollisionType()
{
	if (BoxComponent->GetCollisionEnabled() == ECollisionEnabled::NoCollision)
	{
		BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

