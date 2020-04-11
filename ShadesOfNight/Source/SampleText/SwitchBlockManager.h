// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SwitchBlockManager.generated.h"

UCLASS()
class SAMPLETEXT_API ASwitchBlockManager : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASwitchBlockManager();

	static bool CheckIsActive();

protected:
	virtual void BeginPlay() override;

private:
	static bool IsActive;

	void Switch();

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
