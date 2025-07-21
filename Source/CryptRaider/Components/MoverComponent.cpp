// Fill out your copyright notice in the Description page of Project Settings.


#include "MoverComponent.h"

#include "Components/AudioComponent.h"

#include "CryptRaider/Actors/PressurePlateActor.h"

#include "Kismet/GameplayStatics.h"

#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMoverComponent::UMoverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// Make sure to tick together with Mover Components, and offload the game tick.
	PrimaryComponentTick.TickInterval = 1.0f / 60.0f; // 30 Hz = 1/30 seconds per tick
}


// Called when the game starts
void UMoverComponent::BeginPlay()
{
	Super::BeginPlay();

	// Not validating actor, because there can not be an ActorComponent without an Actor
	ActorToMove = GetOwner();
	DefaultLocation = ActorToMove->GetActorLocation();
	CurrentLocation = DefaultLocation;
}

// Called every frame
void UMoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MoveActor(bShouldMove, DeltaTime);
	
}

void UMoverComponent::SetWantsToOpen()
{
	this->bShouldMove = true;
	this->bIsMovingFinished = false;
}

void UMoverComponent::SetWantsToClose()
{
	this->bShouldMove = false;
	this->bIsMovingFinished = false;
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
		bIsMovingFinished = MoveToLocation(TargetLocation, DeltaTimeSeconds, OpenSpeed);
	}
	else if (!bCanMove)
	{
		bIsMovingFinished = MoveToLocation(DefaultLocation, DeltaTimeSeconds, CloseSpeed);
	}
}


bool UMoverComponent::MoveToLocation(const FVector& End, const float& DeltaTimeSeconds, const float & InterpSpeed)
{
	ActorToMove->SetActorLocation(CurrentLocation);
	
	CurrentLocation = FMath::VInterpTo(
		CurrentLocation,
		End,
		DeltaTimeSeconds,
		InterpSpeed);

	if (CurrentLocation.Equals(End, 0.5f))
	{
		return true;
	}
	return false;
}