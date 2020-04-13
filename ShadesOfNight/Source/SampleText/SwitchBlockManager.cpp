// Fill out your copyright notice in the Description page of Project Settings.

#include "SwitchBlockManager.h"
#include "Components/InputComponent.h"
#include "Runtime/UMG/Public/UMG.h"

// Sets default values
ASwitchBlockManager::ASwitchBlockManager()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//SetupPlayerInputComponent(GetWorld()->GetFirstPlayerController()->InputComponent);
}

bool ASwitchBlockManager::IsActive{ false };

void ASwitchBlockManager::BeginPlay()
{
	SetupPlayerInputComponent(GetWorld()->GetFirstPlayerController()->InputComponent);
}

bool ASwitchBlockManager::CheckIsActive()
{
	return IsActive;
}

void ASwitchBlockManager::Switch()
{
	IsActive = !IsActive;
	UE_LOG(LogTemp, Warning, TEXT("%i"), IsActive)
}

// Called to bind functionality to input
void ASwitchBlockManager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Switch", IE_Pressed, this, &ASwitchBlockManager::Switch);
}