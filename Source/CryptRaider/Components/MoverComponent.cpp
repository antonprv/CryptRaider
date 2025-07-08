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

	ActorToMove = GetOwner();
	
}


// Called every frame
void UMoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MoveActor(bShouldMove);
}

void UMoverComponent::MoveActor(const bool & bCanMove)
{
	if (bIsMovingFinished && !bCanMove)
	{
		return;
	}
	
	CurrentLocation = FMath::VInterpConstantTo(
		CurrentLocation,
		TargetLocation,
		GetWorld()->DeltaTimeSeconds,
		MoveSpeed);

	if (CurrentLocation.Equals(TargetLocation))
	{
		bIsMovingFinished = true;
	}
}
