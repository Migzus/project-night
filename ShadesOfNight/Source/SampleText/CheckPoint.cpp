// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerMovement.h"
#include "Components/BoxComponent.h"
#include "CheckPoint.h"

// Sets default values
ACheckPoint::ACheckPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<UBoxComponent*> _Components;

	GetComponents(_Components);
	for (auto Box : _Components)
	{
		Box->OnComponentBeginOverlap.AddDynamic(this, &ACheckPoint::OnOverlapEnter);
	}

	// Makes the spawnpoint relative to the owner/root (aka. this)
	SpawnPoint += GetActorLocation();
}

void ACheckPoint::OnOverlapEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Set the new checkpoint
	if (!HasCoughtThisCheckpint)
	{
		APlayerMovement* _Player = Cast<APlayerMovement>(OtherActor);

		UE_LOG(LogTemp, Warning, TEXT("Prepare Spawnpoint"))

		if (_Player)
		{
			_Player->SpawnPoint = SpawnPoint;

			UE_LOG(LogTemp, Warning, TEXT("Setting Spawnpoint"))

			HasCoughtThisCheckpint = true;
		}
	}
}


