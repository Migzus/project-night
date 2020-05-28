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
		// Jump Height
		float JumpHeight{ 2300.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		// Grounded speed
		float MaxSpeed{ 1500.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		// Crouched Speed
		float CrouchSpeed{ 600.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		// Speed when in the air
		float AirbornSpeed{ 400.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		// Depending on this variable, the tolerance dictates wether to do a Long jump or a High jump
		float SpecialJumpTolerance{ 100.0f };
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
		// When the player jumps off a wall then this controls the speed/distance away from the wall
		float JumpOffWallSpeed{ 1500.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		// When the player jumps off a wall then this controls the jumpheight 
		float WalljumpHeight{ 2000.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		// The force that pushes the player in the desired direction after pressing the dash button
		float DashForce{ 1300.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CrouchGravityScale{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> PauseMenuPrefab;

	UPROPERTY(BlueprintReadonly)
		bool IsCrouching{ false };
	UPROPERTY(BlueprintReadonly)
		bool IsDashing{ false };
	UPROPERTY(BlueprintReadonly)
		bool IsHidden{ false };

	// current spawnpoint
	FVector SpawnPoint;

	// placeholder
	uint8 CoinsCollected{ 0 };

protected:
	FTimerHandle DashCooldownTimer, CrouchTimer;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Get the camera component that is attached to this player
	class UCameraComponent* Camera;
	class UMeshComponent* Mesh;
	class AActor* PreviousWallReference;
	class UUserWidget* PauseMenuInstance;

	// this variable is set automatically
	float StandardGravityScale{ 0.0f };
	
	bool CanStillHighJump{ false };

	const uint8 AirDashAmount{ 1 };
	uint8 CurrentAirDashCount{ 0 };
	FVector KeepVelocity, InputDirection;

	void MoveX(float value);
	void MoveY(float value);
	void Dash();
	void DashCooldown(float time);
	void StopDashing();
	void JumpLocal();
	void HighJump();
	void LongJump();
	void Crouching();
	void UnCrouching();
	void CanHighJumpLag();
	void GotoBoss();
	void PauseGame();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
