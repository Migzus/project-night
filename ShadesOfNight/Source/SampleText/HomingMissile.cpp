// Fill out your copyright notice in the Description page of Project Settings.


#include "HomingMissile.h"
#include "Components/SphereComponent.h"
#include "PlayerMovement.h"

// Sets default values
AHomingMissile::AHomingMissile()
{
 	// Set this pawn to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USphereComponent* _Sphere{ CreateDefaultSubobject<USphereComponent>("Hitbox") };

	_Sphere->OnComponentBeginOverlap.AddDynamic(this, &AHomingMissile::OnOverlapEnter);
}

void AHomingMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector _DesiredDirection{ FVector::CrossProduct(GetActorLocation(), Target->GetActorLocation()) };

	SetActorRelativeLocation(_DesiredDirection.GetSafeNormal() * DeltaTime);
}

// Called when the game starts or when spawned
void AHomingMissile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHomingMissile::OnOverlapEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("Enemy") && !OtherActor->ActorHasTag("Ignore"))
	{
		if (OtherActor == Target)
		{
			APlayerMovement* _Player{ Cast<APlayerMovement>(OtherActor) };

			if (_Player)
			{
				// change this in the future
				_Player->SetActorLocation(_Player->SpawnPoint);
			}
		}

		Destroy();
	}
}
