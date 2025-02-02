// Copyright INVI1998, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TinyCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UGroomComponent;

UCLASS()
class TINYOPENWORD_API ATinyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATinyCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 数字人
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> FaceComponent;	// 脸部

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void EnableMasterPose(USkeletalMeshComponent* MeshComponent);

private:
	// 弹簧臂组件
	UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;		// 相机摇臂

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> FollowCamera;		// 跟随相机

	UPROPERTY(EditAnywhere)
	TObjectPtr<UGroomComponent> Mustache;	// 胡子

	UPROPERTY(EditAnywhere)
	TObjectPtr<UGroomComponent> Eyelashes;	// 睫毛

	UPROPERTY(EditAnywhere)
	TObjectPtr<UGroomComponent> Fuzz;	// 胡须

	UPROPERTY(EditAnywhere)
	TObjectPtr<UGroomComponent> Eyebrows;	// 眉毛

	UPROPERTY(EditAnywhere)
	TObjectPtr<UGroomComponent> Hair;	// 头发

	UPROPERTY(EditAnywhere)
	TObjectPtr<UGroomComponent> Beard;	// 胡子

	

};
