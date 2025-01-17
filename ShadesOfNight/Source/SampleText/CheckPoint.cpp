// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckPoint.h"
#include "PlayerMovement.h"
#include "Components/BoxComponent.h"

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
}

void ACheckPoint::OnOverlapEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Set the new checkpoint
	if (!HasCoughtThisCheckpint)
	{
		APlayerMovement* _Player = Cast<APlayerMovement>(OtherActor);

		if (_Player)
		{
			_Player->SpawnPoint = GetActorLocation();

			HasCoughtThisCheckpint = true;
		}
	}
}
