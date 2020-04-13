// Fill out your copyright notice in the Description page of Project Settings.


#include "GuardController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AGuardController::AGuardController()
{
	PrimaryActorTick.bCanEverTick = true;

	// Instance components
	SightConfiguration = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>("Perception"));

	// Set the according parameters
	SightConfiguration->SightRadius = SightRadius;
	SightConfiguration->PeripheralVisionAngleDegrees = FieldOfView;
	SightConfiguration->LoseSightRadius = LoseSightRadius;
	SightConfiguration->SetMaxAge(SightAge);

	SightConfiguration->DetectionByAffiliation.bDetectEnemies = true;
	SightConfiguration->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfiguration->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfiguration->GetSenseImplementation());
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AGuardController::OnPawnEnter);
	GetPerceptionComponent()->ConfigureSense(*SightConfiguration);
}

void AGuardController::BeginPlay()
{
	Super::BeginPlay();
}

void AGuardController::OnPossess(APawn* pawn)
{

}

void AGuardController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FRotator AGuardController::GetControlRotation() const
{
	if (GetPawn() == nullptr)
		return FRotator::ZeroRotator;

	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}

void AGuardController::OnPawnEnter(const TArray<AActor*>& DetectedPawns)
{

}
