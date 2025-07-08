// Fill out your copyright notice in the Description page of Project Settings.


#include "MoverComponent.h"

#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMoverComponent::UMoverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMoverComponent::BeginPlay()
{
	Super::BeginPlay();

	// Not validating actor, because there can not be an ActorComponent without an Actor
	ActorToMove = GetOwner();
	GameWorld = GetWorld();
	if (GameWorld == nullptr)
	{
		bShouldMove = false;
	}

	CurrentLocation = ActorToMove->GetActorLocation();
	
}


// Called every frame
void UMoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MoveActor(bShouldMove, GameWorld->GetDeltaSeconds());
}

void UMoverComponent::MoveActor(const bool & bCanMove, const float & DeltaTimeSeconds)
{
	if (bIsMovingFinished && !bCanMove)
	{
		return;
	}

	ActorToMove->SetActorLocation(CurrentLocation);
	
	CurrentLocation = FMath::VInterpConstantTo(
		CurrentLocation,
		TargetLocation,
		DeltaTimeSeconds,
		MoveSpeed);

	if (CurrentLocation.Equals(TargetLocation))
	{
		bIsMovingFinished = true;
	}
}
