// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.


#include "GrabberComponent.h"

#include "DrawDebugHelpers.h"
#include "KismetTraceUtils.h"

#include "Camera/CameraComponent.h"

#include "CryptRaider/Actors/Hotfix/SelectTypeSwitcher.h"

#include "Engine/World.h"

#include "Kismet/GameplayStatics.h"

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

	SubscribeToTypeSwitchers();
	
#if WITH_EDITORONLY_DATA
	DECLARE_LOG_CATEGORY_CLASS(UGrabberComponentLog, Error, Error)
#endif
	
	OwningActor = GetOwner();
	OwnerCamera = OwningActor->FindComponentByClass<UCameraComponent>();
	GrabHandle = OwningActor->FindComponentByClass<UPhysicsHandleComponent>();
	if (OwnerCamera == nullptr)
	{
		bIsValid = false;

#if WITH_EDITORONLY_DATA
		UE_LOG(UGrabberComponentLog, Error, TEXT("No Camera Component found in the owning Actor"))
#endif
		
	}
	else if (GrabHandle == nullptr)
	{
		bIsValid = false;

#if WITH_EDITORONLY_DATA
		UE_LOG(UGrabberComponentLog, Error, TEXT("No Physics Component found in the owning Actor"))
#endif
	}
}

void UGrabberComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (AActor* Actor : TypeSwitchActors)
	{
		ASelectTypeSwitcher* Switcher = Cast<ASelectTypeSwitcher>(Actor);
		Switcher->OnPlayerEnteredArea.RemoveDynamic(this, &UGrabberComponent::SetCanTraceTroughWalls);
	}
	
	Super::EndPlay(EndPlayReason);
}

void UGrabberComponent::SubscribeToTypeSwitchers()
{
	if (GetWorld())
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASelectTypeSwitcher::StaticClass(), TypeSwitchActors);

		if (TypeSwitchActors.Num() > 0)
		{
			for (AActor* Actor : TypeSwitchActors)
			{
				ASelectTypeSwitcher* Switcher = Cast<ASelectTypeSwitcher>(Actor);
				Switcher->OnPlayerEnteredArea.AddDynamic(this, &UGrabberComponent::SetCanTraceTroughWalls);
			}
		}
	}
}

void UGrabberComponent::SetCanTraceTroughWalls(bool bCanTrace)
{
	bCanTraceTroughWalls = bCanTrace;
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
#if WITH_EDITORONLY_DATA
	DECLARE_LOG_CATEGORY_CLASS(UGrabberPhysics, Error, Error)
#endif
	
	if (bHasHit)
	{
		if (!GrabResult.GetComponent()->IsSimulatingPhysics())
		{
#if WITH_EDITORONLY_DATA
			UE_LOG(UGrabberPhysics, Error,
				TEXT("%s is not simulating physics"), *GrabResult.GetActor()->GetActorNameOrLabel())
#endif
			return;
		}

		GrabResult.GetComponent()->WakeAllRigidBodies();
		
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
	if (GrabHandle->GetGrabbedComponent() == nullptr)
	{
		return;
	}

	GrabResult.GetComponent()->WakeAllRigidBodies();
	GrabHandle->ReleaseComponent();
	
	bIsGrabbing = false;
}


void UGrabberComponent::TraceFromCamera(const float& TraceDistance, const float& SphereRadius, FVector& OutStartTrace,
                                        FVector& OutEndTrace, FHitResult& OutHitResult, bool& OutIsHit, const bool& bIsDebugging)
{
	if (GetWorld() == nullptr || !bIsValid)
	{
		return;
	}

#if WITH_EDITORONLY_DATA
	if (bIsDebugging)
	{
		DrawDebug(TraceDistance, GrabRadius, OutStartTrace, OutEndTrace);
	}
#endif
	

	OutStartTrace = OwnerCamera->GetComponentLocation();
	OutEndTrace = OwnerCamera->GetComponentLocation() + OwnerCamera->GetForwardVector() * TraceDistance;

	if (!bCanTraceTroughWalls)
	{
		if (GetWorld()->SweepSingleByChannel(
		OutHitResult,
		OutStartTrace,
		OutEndTrace,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(SphereRadius)))
		{
			OutIsHit = false;
			return;
		}
	}
	
	OutIsHit = GetWorld()->SweepSingleByChannel(
		OutHitResult,
		OutStartTrace,
		OutEndTrace,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(SphereRadius));
	
}

#if WITH_EDITORONLY_DATA
void UGrabberComponent::DrawDebug(const float& TraceDistance, const float& SphereRadius, const FVector& StartTrace,
	const FVector& EndTrace)
{
	// Make sphere trace, that precisely repeats the actual trace in TraceFromCamera
	DrawDebugSweptSphere(
		GetWorld(),
		StartTrace,
		EndTrace,
		SphereRadius,
		FColor::Emerald);

	if (bHasHit)
	{
		// Draw a sphere in the place, where we're touching the object
		DrawDebugSphere(
			GetWorld(),
			GrabResult.Location,
			SphereRadius,
			16,
			FColor::Emerald);
	}
}
#endif