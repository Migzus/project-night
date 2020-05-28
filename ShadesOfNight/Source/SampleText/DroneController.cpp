// Fill out your copyright notice in the Description page of Project Settings.

#include "DroneController.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerMovement.h"
#include "DrawDebugHelpers.h"
#include "HomingMissile.h"
#include "TimerManager.h"

// Sets default values
ADroneController::ADroneController()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CreateDefaultSubobject<UBoxComponent>("Detection Zone");
	DroneMesh = CreateDefaultSubobject<UStaticMeshComponent>("Drone Root");
	//BoxComponent = CreateDefaultSubobject<UBoxComponent>("Movement Component");
}

// Called when the game starts or when spawned
void ADroneController::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(SpawnRate, this, &ADroneController::SpawnMissile, 0.0f, false, MissileCooldown);

	/*TSubclassOf<UBoxComponent> _BoxCollider;
	TArray<UBoxComponent*> _DetectionZone{ GetComponentsByTag(_BoxCollider, "DetectionZone") };

	for (auto Box : _DetectionZone)
	{
		Box->OnComponentBeginOverlap.AddDynamic(this, &ADroneController::OnOverlapEnter);
	}

	TSubclassOf<UStaticMeshComponent> _Mesh;
	TArray<UStaticMeshComponent*> _DroneComponent{ GetComponentsByTag(_Mesh, "DroneRoot") };

	DroneMesh = _DroneComponent[0];*/
}

void ADroneController::OnOverlapEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerMovement* _Player = Cast<APlayerMovement>(OtherActor);

	if (_Player)
	{
		Player = _Player;
		bIsSeekingPlayer = true;
	}
}

// Called every frame
void ADroneController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSeekingPlayer)
	{
		if (IsPlayerInSight())
		{
			if (CurrentMissile == nullptr)
			{
				
			}
		}
	}
}

bool ADroneController::IsPlayerInSight() const
{
	FHitResult _Hit;

	FVector _Start{ GetActorLocation() };
	FVector _End{ Player->GetActorLocation() };

	ECollisionChannel _Ground{ ECollisionChannel::ECC_WorldStatic };

	FCollisionQueryParams _Params{ FCollisionQueryParams() };

	_Params.AddIgnoredActor(this);

	DrawDebugLine(GetWorld(), _Start, _End, FColor::Red, false, 0.1f, 0, 6.0f);

	bool _Statement{ GetWorld()->LineTraceSingleByChannel(_Hit, _Start, _End, _Ground, _Params) };

	return _Statement;
}

void ADroneController::SpawnMissile()
{
	FActorSpawnParameters _ActorParams{};

	 _ActorParams.Owner->SetActorLocation(GetActorLocation());
	 _ActorParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	 AHomingMissile* _NewMissile{ GetWorld()->SpawnActor<AHomingMissile>(MissilePrefab, _ActorParams) };
	 
	 if (_NewMissile)
	 {
		 _NewMissile->SetTarget<APlayerMovement>(Player);
		 CurrentMissile = _NewMissile;
	 }
}
