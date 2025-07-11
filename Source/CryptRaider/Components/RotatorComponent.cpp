// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.


#include "RotatorComponent.h"

// Sets default values for this component's properties
URotatorComponent::URotatorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void URotatorComponent::BeginPlay()
{
	Super::BeginPlay();

	// Not validating actor, because there can not be an ActorComponent without an Actor
	ActorToMove = GetOwner();
	GameWorld = GetWorld();
	if (GameWorld == nullptr)
	{
		bShouldMove = false;
	}

	DefaultRotation = ActorToMove->GetActorRotation();
	CurrentRotation = DefaultRotation;
}


// Called every frame
void URotatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RotateActor(bShouldMove, GameWorld->GetDeltaSeconds());
}

void URotatorComponent::SetShouldMove()
{
	bShouldMove = true;
}

void URotatorComponent::RotateActor(const bool& bCanMove, const float& DeltaTimeSeconds)
{
	if (bIsMovingFinished || FMath::IsNearlyZero(DeltaTimeSeconds))
	{
		return;
	}

	if (bCanMove)
	{
		bIsMovingFinished = RotateToRotation(CurrentRotation, TargetRotation, DeltaTimeSeconds);
	}
	else if (!bCanMove)
	{
		bIsMovingFinished = RotateToRotation(CurrentRotation, DefaultRotation, DeltaTimeSeconds);
	}
}

bool URotatorComponent::RotateToRotation(const FRotator& Start, const FRotator& End, const float & DeltaTimeSeconds)
{
	ActorToMove->SetActorRotation(Start);

	MoveSpeed += Acceleration;
	
	CurrentRotation = FMath::RInterpTo(
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
