// Fill out your copyright notice in the Description page of Project Settings.

#include "SwitchBlockManager.h"
#include "SwitchBlockComponent.h"

// Sets default values for this component's properties
USwitchBlockComponent::USwitchBlockComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void USwitchBlockComponent::BeginPlay()
{
	Super::BeginPlay();

	StartPosition = GetOwner()->GetActorLocation();
	EndPosition += StartPosition;

	switch (StartSwitchType)
	{
	case ESwitchType::ST_BLUE:
		GetOwner()->SetActorLocation(EndPosition);
		break;
	case ESwitchType::ST_RED:
		GetOwner()->SetActorLocation(StartPosition);
		break;
	default:
		break;
	}
}

void USwitchBlockComponent::Blue()
{
	GetOwner()->SetActorLocation(ASwitchBlockManager::CheckIsActive() ? FMath::Lerp(GetOwner()->GetActorLocation(), StartPosition, MoveSpeed) : FMath::Lerp(GetOwner()->GetActorLocation(), EndPosition, MoveSpeed));
}

void USwitchBlockComponent::Red()
{
	GetOwner()->SetActorLocation(!ASwitchBlockManager::CheckIsActive() ? FMath::Lerp(GetOwner()->GetActorLocation(), StartPosition, MoveSpeed) : FMath::Lerp(GetOwner()->GetActorLocation(), EndPosition, MoveSpeed));
}

// Called every frame
void USwitchBlockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (StartSwitchType)
	{
	case ESwitchType::ST_BLUE:
		Blue();
		break;
	case ESwitchType::ST_RED:
		Red();
		break;
	default:
		break;
	}
}

