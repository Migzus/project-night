// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerMovement.h"
#include "Components/InputComponent.h"
#include "Components/MeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "SwitchBlockManager.h"

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

	GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;

	StandardGravityScale = GetCharacterMovement()->GravityScale;

	KeepVelocity = FRotator(InputDirection.GetSafeNormal().Rotation() + Camera->GetComponentRotation()).Vector();

	PauseMenuInstance = CreateWidget<UUserWidget>(GetWorld(), PauseMenuPrefab);

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

	if (GetCharacterMovement()->IsFalling())
	{
		GetCharacterMovement()->GravityScale = IsCrouching ? CrouchGravityScale : StandardGravityScale;
		
		if (IsCrouching)
		{
			CanStillHighJump = true;
		}
	}
	else
	{
		PreviousWallReference = nullptr;
		
		if (!IsDashing)
		{
			CurrentAirDashCount = 0;
		}
	}

	GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->IsFalling() ? AirbornSpeed : IsCrouching ? CrouchSpeed : MaxSpeed;

	KeepVelocity = InputDirection.IsZero() ? KeepVelocity : FRotator(InputDirection.GetSafeNormal().Rotation() + Camera->GetComponentRotation()).Vector();
	KeepVelocity.Z = _CurrentVelocity.Z = 0.0f;

	FRotator _SmoothRotation{ FMath::Lerp(Mesh->GetRelativeRotation(), KeepVelocity.Rotation(), 0.2f) };

	Mesh->SetWorldRotation(_SmoothRotation);
}

void APlayerMovement::MoveX(float value)
{
	FVector _Direction{ FVector::ZeroVector };

	if (Camera != nullptr)
	{
		const FRotator _YawRotation{ FRotator(0.0f, Camera->GetComponentRotation().Yaw, 0.0f) };

		_Direction = FRotationMatrix(_YawRotation).GetUnitAxis(EAxis::X);
	}

	//KeepVelocity.X = value != 0.0f ? value : KeepVelocity.X;
	InputDirection.X = value;

	//UE_LOG(LogTemp, Warning, TEXT("[%f, %f, %f]"), _Direction.X, _Direction.Y, _Direction.Z)

	AddMovementInput(_Direction, value);
}

void APlayerMovement::MoveY(float value)
{
	FVector _Direction{ FVector::ZeroVector };

	if (Camera != nullptr)
	{
		const FRotator _YawRotation{ FRotator(0.0f, Camera->GetComponentRotation().Yaw, 0.0f) };

		_Direction = FRotationMatrix(_YawRotation).GetUnitAxis(EAxis::Y);
	}

	//KeepVelocity.Y = value != 0.0f ? value : KeepVelocity.Y;
	InputDirection.Y = value;

	AddMovementInput(_Direction, value);
}

void APlayerMovement::Dash()
{
	if (AirDashAmount > CurrentAirDashCount)
	{
		GetCharacterMovement()->AddForce(KeepVelocity.GetSafeNormal() * 100000.0f * DashForce);
		CurrentAirDashCount++;
		DashCooldown(0.2f);
	}
}

void APlayerMovement::DashCooldown(float time)
{
	IsDashing = true;
	GetWorldTimerManager().SetTimer(DashCooldownTimer, this, &APlayerMovement::StopDashing, time, false);
}

void APlayerMovement::StopDashing()
{
	IsDashing = false;
}

void APlayerMovement::JumpLocal()
{
	// TODO
	// include a check beneath the player so that they can still "JUMP", even if they technically are above ground

	///////////////////////////////////////////////////////////

	bPressedJump = true;

	// if our character is still falling we want to enable wall jump
	if (GetCharacterMovement()->IsFalling())
	{
		for (size_t i = 0; i < 3; i++)
		{
			float _Spread{ PI / 2.25 };
			float _Angle{ KeepVelocity.HeadingAngle() - _Spread + _Spread * i };

			// prepare the ray results
			FHitResult _Hit;
			// ray origin
			FVector _Start{ GetActorLocation() };
			// ray end-point
			FVector _End{ _Start + FVector(FMath::Cos(_Angle), FMath::Sin(_Angle), 0.0f) * 100.0f };
			//FVector _End{ _Start + (KeepVelocity.GetSafeNormal() * 100.0f) };

			ECollisionChannel _WallJumpChannel{ ECollisionChannel::ECC_GameTraceChannel1 };

			//DrawDebugLine(GetWorld(), _Start, _End, FColor::Red, false, 0.1f, 0, 6.0f);

			// cast a ray to see if there is any wall with the WallJump tag
			if (GetWorld()->LineTraceSingleByChannel(_Hit, _Start, _End, _WallJumpChannel))
			{
				if (_Hit.GetActor() != PreviousWallReference)
				{
					// We need the direction of the ray
					FVector _DirectionalVector{ _End - _Start };

					_DirectionalVector.Z = 0.0f;
					_DirectionalVector = _DirectionalVector.GetSafeNormal();

					// mirror the direction vector
					_DirectionalVector = _DirectionalVector.MirrorByVector(_Hit.ImpactNormal);
					_DirectionalVector *= JumpOffWallSpeed;
					_DirectionalVector += FVector::UpVector * WalljumpHeight;

					// finally we set the our new velocity to the directional vector, included force
					GetCharacterMovement()->Velocity = _DirectionalVector;

					PreviousWallReference = _Hit.GetActor();

					break;
				}
			}
		}
	}
	
	if (!GetCharacterMovement()->IsFalling())
	{
		if (IsCrouching)
		{
			HighJump();
			return;
		}

		if (IsDashing)
		{
			LongJump();
			return;
		}

		// otherwise just do a standard jump
		//Jump();
		GetCharacterMovement()->Velocity.Z = JumpHeight;
	}
}

void APlayerMovement::HighJump()
{
	FVector _Direction{ FVector(KeepVelocity.GetSafeNormal().X * HighJumpSpeed, KeepVelocity.GetSafeNormal().Y * HighJumpSpeed, HighJumpHeight) };

	_Direction.X *= -1.0f;
	_Direction.Y *= -1.0f;

	GetCharacterMovement()->Velocity = _Direction;

	//GetCharacterMovement()->AddForce(_Direction * 100000.0f);
}

void APlayerMovement::LongJump()
{
	FVector _Direction{ FVector(KeepVelocity.GetSafeNormal().X * LongJumpSpeed, KeepVelocity.GetSafeNormal().Y * LongJumpSpeed, LongJumpHeight) };

	CanStillHighJump = false;

	GetCharacterMovement()->Velocity = _Direction;

	//GetCharacterMovement()->AddForce(_Direction * 100000.0f);
}

void APlayerMovement::Crouching()
{
	GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
	IsCrouching = true;
}

void APlayerMovement::UnCrouching()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
	IsCrouching = false;
	CanStillHighJump = false;
	//GetWorldTimerManager().SetTimer(CrouchTimer, this, &APlayerMovement::CanHighJumpLag, 0.17f, false);
}

void APlayerMovement::CanHighJumpLag()
{
	CanStillHighJump = false;
}

void APlayerMovement::GotoBoss()
{
	UGameplayStatics::OpenLevel(this, "Boss_lvl");
}

void APlayerMovement::PauseGame()
{
	bool _IsPaused{ UGameplayStatics::IsGamePaused(GetWorld()) };

	if (!_IsPaused)
	{
		if (PauseMenuInstance)
		{
			PauseMenuInstance->AddToViewport();
		}
	}
	else
	{
		PauseMenuInstance->RemoveFromViewport();
	}

	UGameplayStatics::SetGamePaused(GetWorld(), !_IsPaused);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = !_IsPaused;
}

// Called to bind functionality to input
void APlayerMovement::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerMovement::JumpLocal);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerMovement::StopJumping);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerMovement::Crouching);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APlayerMovement::UnCrouching);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &APlayerMovement::Dash);

	PlayerInputComponent->BindAxis("MoveX", this, &APlayerMovement::MoveX);
	PlayerInputComponent->BindAxis("MoveY", this, &APlayerMovement::MoveY);

	PlayerInputComponent->BindAction("Cheat", IE_Pressed, this, &APlayerMovement::GotoBoss);
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &APlayerMovement::PauseGame);
}

