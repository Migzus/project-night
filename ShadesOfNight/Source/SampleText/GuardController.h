// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GuardController.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLETEXT_API AGuardController : public AAIController
{
	GENERATED_BODY()
	
public:
	AGuardController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float SightAge{ 4.3f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float SightRadius{ 750.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float FieldOfView{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		class UAISenseConfig_Sight* SightConfiguration;

protected:
	float LoseSightRadius{ SightRadius + 30.0f };

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* pawn) override;
	virtual void Tick(float DeltaTime) override;
	virtual FRotator GetControlRotation() const override;

	UFUNCTION()
		void OnPawnEnter(const TArray<AActor*>& DetectedPawns);
};
