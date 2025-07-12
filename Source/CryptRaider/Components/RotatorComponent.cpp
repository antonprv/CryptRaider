// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.


#include "RotatorComponent.h"

#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
URotatorComponent::URotatorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// Make sure to tick together with Mover Components, and offload the game tick.
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;
}


// Called when the game starts
void URotatorComponent::BeginPlay()
{
	Super::BeginPlay();

	// Not validating actor, because there can not be an ActorComponent without an Actor
	ActorToMove = GetOwner();
	DefaultRotation = ActorToMove->GetActorRotation();
	CurrentRotation = DefaultRotation;
}


// Called every frame
void URotatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RotateActor(bShouldMove, DeltaTime);
}


void URotatorComponent::SetShouldMove()
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

void URotatorComponent::SetShouldNotMove()
{
	this->bShouldMove = false;
	this->bIsMovingFinished = false;
}


bool URotatorComponent::GetShouldMove() const
{
	return bShouldMove;
}


void URotatorComponent::RotateActor(const bool& bCanMove, const float& DeltaTimeSeconds)
{
	if (bIsMovingFinished || FMath::IsNearlyZero(DeltaTimeSeconds))
	{
		return;
	}

	if (bCanMove)
	{
		bIsMovingFinished = RotateToRotation(TargetRotation, DeltaTimeSeconds);
	}
	else if (!bCanMove)
	{
		bIsMovingFinished = RotateToRotation(DefaultRotation, DeltaTimeSeconds);
	}
}


bool URotatorComponent::RotateToRotation(const FRotator& End, const float & DeltaTimeSeconds)
{
	ActorToMove->SetActorRotation(CurrentRotation);
	
	CurrentRotation = FMath::RInterpTo(
		CurrentRotation,
		End,
		DeltaTimeSeconds,
		MoveSpeed);
	
	if (CurrentRotation.Equals(End, 0.5f))
	{
		return true;
	}
	return false;
}
