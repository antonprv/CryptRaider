// Fill out your copyright notice in the Description page of Project Settings.


#include "MoverComponent.h"

#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMoverComponent::UMoverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// Make sure to tick together with Mover Components, and offload the game tick.
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;

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

	DefaultLocation = ActorToMove->GetActorLocation();
	CurrentLocation = DefaultLocation;
}


// Called every frame
void UMoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MoveActor(bShouldMove, GameWorld->GetDeltaSeconds());
	
}

void UMoverComponent::SetShouldMove()
{
	bShouldMove = true;
}

void UMoverComponent::SetShouldNotMove()
{
	bShouldMove = false;
}


bool UMoverComponent::GetShouldMove() const
{
	return bShouldMove;
}


void UMoverComponent::MoveActor(const bool & bCanMove, const float & DeltaTimeSeconds)
{
	if (bIsMovingFinished || FMath::IsNearlyZero(DeltaTimeSeconds))
	{
		return;
	}

	if (bCanMove)
	{
		bIsMovingFinished = MoveToLocation(CurrentLocation, TargetLocation, DeltaTimeSeconds);
	}
	else if (!bCanMove)
	{
		bIsMovingFinished = MoveToLocation(CurrentLocation, DefaultLocation, DeltaTimeSeconds);
	}
}


bool UMoverComponent::MoveToLocation(FVector& Start, const FVector& End, const float& DeltaTimeSeconds) const
{
	ActorToMove->SetActorLocation(Start);
	
	Start = FMath::VInterpTo(
		Start,
		End,
		DeltaTimeSeconds,
		MoveSpeed);

	if (Start.Equals(End))
	{
		return true;
	}
	return false;
}
