// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/InputComponent.h"
#include "Components/MeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "SwitchBlockManager.h"
#include "PlayerMovement.h"

// Sets default values
APlayerMovement::APlayerMovement()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlayerMovement::BeginPlay()
{
	Super::BeginPlay();
	
	// Prepare an array to get the camera
	TArray<UCameraComponent*> _CameraComponent;
	TArray<UMeshComponent*> _MeshComponent;

	// Get the all of the camera components
	GetComponents(_CameraComponent);
	// Just take the first camera controller and set that as the default camera component
	Camera = _CameraComponent[0];

	// since we always expect one component of the mesh type
	GetComponents(_MeshComponent);

	Mesh = _MeshComponent[0];

	//ASwitchBlockManager::CheckIsActive();

	// THIS WILL BE REMOVED //
	SpawnPoint = FVector::UpVector * 15.0f;
}

// Called every frame
void APlayerMovement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector _CurrentVelocity{ GetCharacterMovement()->Velocity.GetSafeNormal() };
	//FRotator _Rotation{ FRotator(_CurrentVelocity.X, _CurrentVelocity.Y, 0.0f) };

	/////////////////////////////
	// THIS WILL BE MOVED
	/////////////////////////////

	if (GetActorLocation().Z <= KillBorder)
	{
		SetActorLocation(SpawnPoint);
	}

	/////////////////////////////

	//FVector _ { Camera->GetComponentRotation().Vector() };
	//UE_LOG(LogTemp, Warning, TEXT("[%f, %f, %f]"), _.X, _.Y, _.Z)

	KeepVelocity = InputDirection.IsZero() ? KeepVelocity : FRotator(InputDirection.GetSafeNormal().Rotation() + Camera->GetComponentRotation()).Vector();
	KeepVelocity.Z = _CurrentVelocity.Z = 0.0f;

	Mesh->SetWorldRotation(_CurrentVelocity.Rotation());
}

void APlayerMovement::MoveX(float value)
{
	FVector _Direction{ FVector::ZeroVector };

	if (IsCameraForward && Camera != nullptr)
	{
		const FRotator _YawRotation{ FRotator(0.0f, Camera->GetComponentRotation().Yaw, 0.0f) };

		_Direction = FRotationMatrix(_YawRotation).GetUnitAxis(EAxis::X);
	}
	else
	{
		_Direction = FVector::ForwardVector;
	}

	//KeepVelocity.X = value != 0.0f ? value : KeepVelocity.X;
	InputDirection.X = value;

	//UE_LOG(LogTemp, Warning, TEXT("[%f, %f, %f]"), _Direction.X, _Direction.Y, _Direction.Z)

	AddMovementInput(_Direction, value);
}

void APlayerMovement::MoveY(float value)
{
	FVector _Direction{ FVector::ZeroVector };

	if (IsCameraForward && Camera != nullptr)
	{
		const FRotator _YawRotation{ FRotator(0.0f, Camera->GetComponentRotation().Yaw, 0.0f) };

		_Direction = FRotationMatrix(_YawRotation).GetUnitAxis(EAxis::Y);
	}
	else
	{
		_Direction = FVector::RightVector;
	}

	//KeepVelocity.Y = value != 0.0f ? value : KeepVelocity.Y;
	InputDirection.Y = value;

	AddMovementInput(_Direction, value);
}

void APlayerMovement::JumpLocal()
{
	// TODO
	// include a check beneath the player so that they can still "JUMP", even if they chenically are above ground

	///////////////////////////////////////////////////////////

	// if our character is still falling we want to enable wall jump
	// (if we have only used one jump and wall jump right after, we have another jump, however if the player has already used
	// their double jump then they will wall jump but fall after that)
	if (GetCharacterMovement()->IsFalling())
	{
		// prepare the ray results
		FHitResult _Hit;
		// ray origin
		FVector _Start{ GetActorLocation() };
		// ray end-point
		FVector _End{ _Start + (KeepVelocity * 100.0f) };
		//FVector _End{ _Start + (KeepVelocity.GetSafeNormal() * 100.0f) };

		ECollisionChannel _WallJumpChannel{ ECollisionChannel::ECC_GameTraceChannel1 };
	    
		DrawDebugLine(GetWorld(), _Start, _End, FColor::Red, false, 0.1f, 0, 6.0f);

		// cast a ray to see if there is any wall with the WallJump tag
		if (GetWorld()->LineTraceSingleByChannel(_Hit, _Start, _End, _WallJumpChannel))
		{
			// We need the direction of the ray
			FVector _DirectionalVector{ _End - _Start };

			_DirectionalVector.Z = 0.0f;
			_DirectionalVector = _DirectionalVector.GetSafeNormal();

			// mirror the direction vector
			_DirectionalVector = _DirectionalVector.MirrorByVector(_Hit.ImpactNormal);
			_DirectionalVector *= 2000;
			_DirectionalVector += FVector(0.0f, 0.0f, 1500.0f);

			// finally we set the our new velocity to the directional vector, included force
			GetCharacterMovement()->Velocity = _DirectionalVector;
		}
	}
	
	if (GetCharacterMovement()->IsCrouching() && !GetCharacterMovement()->IsFalling())
	{
		bPressedJump = true;

		if (GetCharacterMovement()->Velocity.IsNearlyZero())
		{
			HighJump();
			//UE_LOG(LogTemp, Warning, TEXT("HIGH JUMP!"))
		}
		else
		{
			LongJump();
			//UE_LOG(LogTemp, Warning, TEXT("LONG JUMP!!"))
		}
	}
	else
	{
		Jump();
	}
}

void APlayerMovement::HighJump()
{
	FVector _Direction{ FVector(KeepVelocity.GetSafeNormal().X * HighJumpSpeed, KeepVelocity.GetSafeNormal().Y * HighJumpSpeed, HighJumpHeight) };

	_Direction.X *= -1.0f;
	_Direction.Y *= -1.0f;

	//GetCharacterMovement()->Velocity.Z = 6000.0f;
	//JumpKeyHoldTime = 0.0f;
	GetCharacterMovement()->AddForce(_Direction * 100000.0f);

}

void APlayerMovement::LongJump()
{
	FVector _Direction{ FVector(KeepVelocity.GetSafeNormal().X * LongJumpSpeed, KeepVelocity.GetSafeNormal().Y * LongJumpSpeed, LongJumpHeight) };

	//GetCharacterMovement()->AddInputVector(_Direction * 10000000.0f);
	GetCharacterMovement()->AddForce(_Direction * 100000.0f);
}

void APlayerMovement::Crouching()
{
	Super::Crouch();
}

void APlayerMovement::UnCrouching()
{
	Super::UnCrouch();
}

// Called to bind functionality to input
void APlayerMovement::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerMovement::JumpLocal);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerMovement::StopJumping);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerMovement::Crouching);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APlayerMovement::UnCrouching);

	PlayerInputComponent->BindAxis("MoveX", this, &APlayerMovement::MoveX);
	PlayerInputComponent->BindAxis("MoveY", this, &APlayerMovement::MoveY);
}

