// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SwitchBlockComponent.generated.h"

UENUM(BlueprintType)
enum class ESwitchType : uint8 {
	ST_BLUE UMETA(DisplayName = "Blue"),
	ST_RED UMETA(DisplayName = "Red")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SAMPLETEXT_API USwitchBlockComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USwitchBlockComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		// This defines what type of switch type it always will be during runtime.
		// Changing this in-game will not do anything...
		ESwitchType StartSwitchType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		// This is where the endpostion of the block is.
		// The startposition is already decided upon where the location of this component's owner is located.
		FVector EndPosition;

protected:
	FVector StartPosition;

	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float MoveSpeed{ 0.045f };

	void Blue();
	void Red();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
