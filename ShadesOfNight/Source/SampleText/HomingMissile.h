// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "HomingMissile.generated.h"

UCLASS()
class SAMPLETEXT_API AHomingMissile : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AHomingMissile();
	
	virtual void Tick(float DeltaTime) override;

	template<class T> void SetTarget(T* pTarget)
	{
		if (!Cast<AActor>(Target))
		{
			return;
		}

		this->Target = Target;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	class AActor* Target;

	UFUNCTION()
		void OnOverlapEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
