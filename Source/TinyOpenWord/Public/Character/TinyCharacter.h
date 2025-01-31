// Copyright INVI1998, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TinyCharacter.generated.h"

UCLASS()
class TINYOPENWORD_API ATinyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATinyCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

private:

};
