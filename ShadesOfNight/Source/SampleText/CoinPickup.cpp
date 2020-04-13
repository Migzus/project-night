// Fill out your copyright notice in the Description page of Project Settings.

#include "CoinPickup.h"
#include "PlayerMovement.h"
#include "Components/SphereComponent.h"

// Sets default values
ACoinPickup::ACoinPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACoinPickup::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<USphereComponent*> _Components;

	GetComponents(_Components);
	for (auto Sphere : _Components)
	{
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACoinPickup::OnOverlapEnter);
	}
}

void ACoinPickup::OnOverlapEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		APlayerMovement* _Player = Cast<APlayerMovement>(OtherActor);

		if (_Player)
		{
			_Player->CoinsCollected++;

			UE_LOG(LogTemp, Warning, TEXT("%i"), _Player->CoinsCollected)

			Destroy();
		}
	}
}

// Called every frame
void ACoinPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(FRotator(0.0f, RandRotationSpeed * DeltaTime, 0.0f));
}
