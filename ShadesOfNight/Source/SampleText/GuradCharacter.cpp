// Fill out your copyright notice in the Description page of Project Settings.

#include "GuradCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AGuradCharacter::AGuradCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
}

// Called when the game starts or when spawned
void AGuradCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGuradCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGuradCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

