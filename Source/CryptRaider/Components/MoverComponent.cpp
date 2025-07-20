// Fill out your copyright notice in the Description page of Project Settings.


#include "MoverComponent.h"

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

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APressurePlateActor::StaticClass(), TriggerActors);
	for (AActor* Actor : TriggerActors)
	{
		if (APressurePlateActor* PressurePlateActor = Cast<APressurePlateActor>(Actor))
		{
			PressurePlateActor->OnPPTriggered.AddDynamic(this, &UMoverComponent::HandlePressurePlate);
		}
	}
}

void UMoverComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (AActor* Actor : TriggerActors)
	{
		if (APressurePlateActor* PressurePlateActor = Cast<APressurePlateActor>(Actor))
		{
			PressurePlateActor->OnPPTriggered.RemoveDynamic(this, &UMoverComponent::HandlePressurePlate);
		}
	}
	
	Super::EndPlay(EndPlayReason);
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


void UMoverComponent::HandlePressurePlate(ETriggerDirection TriggerDirection)
{
	DECLARE_LOG_CATEGORY_CLASS(LogUMoverComponent, Warning, Warning)
	
	switch (TriggerDirection)
	{
	case ETriggerDirection::Open:
		PlaySound(MoveStartSound);
		break;
	case ETriggerDirection::Close:
		PlaySound(MoveEndSound);
		break;
	default:
			UE_LOG(LogUMoverComponent, Warning, TEXT("Got Inappropriate value from UTriggerComponent"))
	}
}

void UMoverComponent::PlaySound(USoundBase* SoundToPlay) const
{
	if (MoveStartSound && GetWorld())
	{
		UGameplayStatics::PlaySoundAtLocation(
			this, 
			SoundToPlay, 
			GetOwner()->GetActorLocation() // Play at the owner's location
		);
	}
}
