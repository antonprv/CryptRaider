// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.


#include "TriggerComponent.h"

#include "AssetTypeCategories.h"

#include "CryptRaider/Components/Interfaces/IMovable.h"

#include "UObject/FastReferenceCollector.h"


// Sets default values for this component's properties
UTriggerComponent::UTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.
	// You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// Make sure to tick together with Mover Components, and offload the game tick.
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;

	Super::SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// Set default collision responses
	
	FCollisionResponseContainer ResponseContainer;
	ResponseContainer.GameTraceChannel2 = ECR_Overlap;
	ResponseContainer.PhysicsBody = ECR_Overlap;
	ResponseContainer.Pawn = ECR_Overlap;
	Super::SetCollisionResponseToChannels(ResponseContainer);
	
	
	SetGenerateOverlapEvents(true);
	
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	KeyActor = GetFittingActor(KeyActors);
	if (KeyActor != nullptr)
	{
		bWantsToTrigger = true;
	}
	else if (KeyActor == nullptr)
	{
		bWantsToTrigger = false;
	}
}

bool UTriggerComponent::GetWantsToTrigger() const
{
	return bWantsToTrigger;
}


AActor* UTriggerComponent::GetFittingActor(TArray<AActor*>& OverlappingActors) const
{
	this->GetOverlappingActors(OverlappingActors);
	for (AActor* Actor : OverlappingActors)
	{
		if (Actor->ActorHasTag(KeyTag))
		{
			DebugShowFitActor(Actor);
			return Actor;
		}
		else if (Actor->FindComponentByTag<UStaticMeshComponent>(KeyTag) != nullptr)
		{
			DebugShowFitActorWithComponent(Actor);
			return Actor;
		}
	}
	return nullptr;
}

void UTriggerComponent::TriggerMover (UObject* IMovableActor)
{
	IIMovable* IMovableCasted = Cast<IIMovable>(IMovableActor);
	if (IMovableCasted != nullptr)
	{
		IMovableCasted->SetShouldMove();
	}
}

void UTriggerComponent::DebugShowFitActor(const AActor* Key) const
{
	if (bIsDebugEnabled)
	{
		UE_LOG(LogTemp, Display, TEXT("%s has key as %s Actor"),
				*GetOwner()->GetActorNameOrLabel(), *Key->GetActorNameOrLabel())
	}
}

void UTriggerComponent::DebugShowFitActorWithComponent(const AActor* Key) const
{
	if (bIsDebugEnabled)
	{
		UE_LOG(LogTemp, Display, TEXT("%s has key as %s's Component"),
					*GetOwner()->GetActorNameOrLabel(), *Key->GetActorNameOrLabel())
	}
}