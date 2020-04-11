// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerMovement.generated.h"

UCLASS()
class SAMPLETEXT_API APlayerMovement : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerMovement();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		// The instant force that pushes the player upward (pushes Z)
		float HighJumpHeight{ 300.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		// The instant force that pushes the player backwards from it's origin (pushes X and Y)
		float HighJumpSpeed{ 100.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		// The instant force that pushes the player after executing the Long Jump Command Combination (pushes Z)
		float LongJumpHeight{ 170.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		// The instant force that pushes the player after executing the Long Jump Command Combination (pushes X and Y)
		float LongJumpSpeed{ 900.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		// (Deprecated, this will be removed)
		// If the player reaches bleow this limit, then we respawn.
		float KillBorder{ -100.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		// (Deprecated, this will be removed)
		// A toggle between two different types of control types.
		// One is dependent on the rotation of the camera, the other isn't.
		bool IsCameraForward{ false };

	FVector SpawnPoint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Get the camera component that is attached to this player
	class UCameraComponent* Camera;
	class UMeshComponent* Mesh;
	
	FVector KeepVelocity, InputDirection;

	void MoveX(float value);
	void MoveY(float value);
	void JumpLocal();
	void HighJump();
	void LongJump();
	void Crouching();
	void UnCrouching();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
