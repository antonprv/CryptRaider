// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine/World.h"

#include "GrabberComponent.h"
#include "Camera/CameraComponent.h"

#include "DrawDebugHelpers.h"


// Sets default values for this component's properties
UGrabberComponent::UGrabberComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabberComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwningActor = GetOwner();
	OwnerCamera = OwningActor->FindComponentByClass<UCameraComponent>();
	if (OwnerCamera == nullptr)
	{
		bIsValid = false;
		UE_LOG(LogTemp, Error, TEXT("No Camera Component found in the owning Actor"))
	}
}


// Called every frame
void UGrabberComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// DrawDebug(GrabDistabce, bIsDebugEnabled);
}

void UGrabberComponent::TraceFromCamera(const float& TraceDistance, const float& SphereRadius, FHitResult& OutHitResult, bool& OutIsHit, const bool& Debug)
{
	if (GetWorld() == nullptr || !bIsValid)
	{
		return;
	}

	if (Debug)
	{
		DrawDebug(TraceDistance, DebugLineThickness);
	}
	
	OutIsHit = GetWorld()->SweepSingleByChannel(
		OutHitResult,
		OwnerCamera->GetComponentLocation(),
		OwnerCamera->GetComponentLocation() + OwnerCamera->GetForwardVector() * TraceDistance,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(SphereRadius));
	
}

void UGrabberComponent::DrawDebug(const float& TraceDistance, const float& LineThickness) const
{
	DrawDebugLine(
		GetWorld(),
		OwnerCamera->GetComponentLocation(),
		OwnerCamera->GetComponentLocation() + OwnerCamera->GetForwardVector() * TraceDistance,
		FColor::Emerald,
		false,
		-1,
		0,
		LineThickness);
}
