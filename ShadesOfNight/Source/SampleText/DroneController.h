// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DroneController.generated.h"

UCLASS()
class SAMPLETEXT_API ADroneController : public APawn
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		// The time it takes, when the player is in
		// view; before spawning the next missile
		float MissileCooldown{ 2.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		// The offset from where this Drone is placed, it will
		// move to this offset when the player is in range
		FVector MoveToDetectPos{ 70.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<class AHomingMissile> MissilePrefab;

	// Sets default values for this pawn's properties
	ADroneController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnOverlapEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	FTimerHandle SpawnRate;

	//class UBoxComponent* BoxComponent;
	class UStaticMeshComponent* DroneMesh;
	class APlayerMovement* Player;
	class AHomingMissile* CurrentMissile;
	FRotator CurrentAimDirection;
	bool bIsSeekingPlayer{ false };

	bool IsPlayerInSight() const;
	void SpawnMissile();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
