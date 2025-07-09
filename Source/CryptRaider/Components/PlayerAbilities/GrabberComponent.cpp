// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.


#include "Engine/World.h"

#include "GrabberComponent.h"
#include "Camera/CameraComponent.h"

#include "DrawDebugHelpers.h"
#include "KismetTraceUtils.h"

#include "PhysicsEngine/PhysicsHandleComponent.h"


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

	DECLARE_LOG_CATEGORY_CLASS(UGrabberComponentLog, Error, Error)
	
	OwningActor = GetOwner();
	OwnerCamera = OwningActor->FindComponentByClass<UCameraComponent>();
	GrabHandle = OwningActor->FindComponentByClass<UPhysicsHandleComponent>();
	if (OwnerCamera == nullptr)
	{
		bIsValid = false;
		
		UE_LOG(UGrabberComponentLog, Error, TEXT("No Camera Component found in the owning Actor"))
	}
	else if (GrabHandle == nullptr)
	{
		bIsValid = false;
		UE_LOG(UGrabberComponentLog, Error, TEXT("No Physics Component found in the owning Actor"))
	}
}


// Called every frame
void UGrabberComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	TraceFromCamera(
		GrabDistance, GrabRadius,
		GrabStart, GrabEnd,
		GrabResult, bHasHit,
		bIsDebugEnabled);

	if (bIsGrabbing)
	{
		KeepGrabbing();
	}
}

void UGrabberComponent::Grab()
{
	DECLARE_LOG_CATEGORY_CLASS(UGrabberPhysics, Error, Error)
	
	if (bHasHit)
	{
		if (!GrabResult.GetComponent()->IsSimulatingPhysics())
		{
			UE_LOG(UGrabberPhysics, Error,
				TEXT("%s is not simulating physics"), *GrabResult.GetActor()->GetActorNameOrLabel())
			return;
		}
		
		GrabHandle->GrabComponentAtLocationWithRotation(
			GrabResult.GetComponent(),
			NAME_None,
			GrabEnd,
			OwnerCamera->GetComponentRotation());

		bIsGrabbing = true;
	}
}

void UGrabberComponent::KeepGrabbing() const
{
	GrabHandle->SetTargetLocationAndRotation(GrabEnd, OwningActor->GetActorRotation());
}

void UGrabberComponent::Release()
{
	if (GrabHandle->GetGrabbedComponent() != nullptr)
	{
		GrabHandle->ReleaseComponent();
	}
	bIsGrabbing = false;
}


void UGrabberComponent::TraceFromCamera(const float& TraceDistance, const float& SphereRadius, FVector& OutStartTrace,
	FVector& OutEndTrace, FHitResult& OutHitResult, bool& OutIsHit, const bool& bIsDebugging)
{
	if (GetWorld() == nullptr || !bIsValid)
	{
		return;
	}

	if (bIsDebugging)
	{
		DrawDebug(TraceDistance, GrabRadius, OutStartTrace, OutEndTrace);
	}

	OutStartTrace = OwnerCamera->GetComponentLocation();
	OutEndTrace = OwnerCamera->GetComponentLocation() + OwnerCamera->GetForwardVector() * TraceDistance;
	
	OutIsHit = GetWorld()->SweepSingleByChannel(
		OutHitResult,
		OutStartTrace,
		OutEndTrace,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(SphereRadius));
	
}

void UGrabberComponent::DrawDebug(const float& TraceDistance, const float& SphereRadius, const FVector& StartTrace,
	const FVector& EndTrace)
{
	DrawDebugSweptSphere(
		GetWorld(),
		StartTrace,
		EndTrace,
		SphereRadius,
		FColor::Emerald);

	if (bHasHit)
	{
		DrawDebugSphere(
			GetWorld(),
			GrabResult.Location,
			SphereRadius,
			16,
			FColor::Emerald);
	}
}