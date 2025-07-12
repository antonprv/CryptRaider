// Fill out your copyright notice in the Description page of Project Settings.


#include "MoverComponent.h"

#include "Kismet/GameplayStatics.h"

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
	DefaultLocation = ActorToMove->GetActorLocation();
	CurrentLocation = DefaultLocation;
}


// Called every frame
void UMoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MoveActor(bShouldMove, DeltaTime);
	
}

void UMoverComponent::SetShouldMove()
{
	this->bShouldMove = true;
	this->bIsMovingFinished = false;

	if (MoveStartSound && GetWorld())
	{
		UGameplayStatics::PlaySoundAtLocation(
			this, 
			MoveStartSound, 
			GetOwner()->GetActorLocation() // Play at the owner's location
		);
	}
}

void UMoverComponent::SetShouldNotMove()
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
		bIsMovingFinished = MoveToLocation(TargetLocation, DeltaTimeSeconds);
	}
	else if (!bCanMove)
	{
		bIsMovingFinished = MoveToLocation(DefaultLocation, DeltaTimeSeconds);
	}
}


bool UMoverComponent::MoveToLocation(const FVector& End, const float& DeltaTimeSeconds)
{
	ActorToMove->SetActorLocation(CurrentLocation);
	
	CurrentLocation = FMath::VInterpTo(
		CurrentLocation,
		End,
		DeltaTimeSeconds,
		MoveSpeed);

	if (CurrentLocation.Equals(End, 0.5f))
	{
		return true;
	}
	return false;
}
