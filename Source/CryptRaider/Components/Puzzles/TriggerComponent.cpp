// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.


#include "TriggerComponent.h"

#include "AssetTypeCategories.h"

#include "CryptRaider/Components/Interfaces/Movable.h"

#include "UObject/FastReferenceCollector.h"


// Sets default values for this component's properties
UTriggerComponent::UTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.
	// You can turn these features
	// off to improve performance if you don't need them.

	Super::SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// Set default collision responses
	
	FCollisionResponseContainer ResponseContainer;
	ResponseContainer.GameTraceChannel2 = ECR_Overlap;
	ResponseContainer.PhysicsBody = ECR_Overlap;
	ResponseContainer.Pawn = ECR_Overlap;
	Super::SetCollisionResponseToChannels(ResponseContainer);
	
	
	SetGenerateOverlapEvents(true);

	OnComponentBeginOverlap.AddDynamic(this, &UTriggerComponent::OnKeyBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UTriggerComponent::OnKeyEndOverlap);
	
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTriggerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	OnComponentBeginOverlap.RemoveDynamic(this, &UTriggerComponent::OnKeyBeginOverlap);
	OnComponentEndOverlap.RemoveDynamic(this, &UTriggerComponent::OnKeyEndOverlap);
	OnDoorOpen.Clear();
	
	Super::EndPlay(EndPlayReason);
}

void UTriggerComponent::TriggerMover (const TScriptInterface<IMovable> IMovableActor) const
{
	if (!IMovableActor)
	{
		UE_LOG(LogTemp, Error, TEXT("UTriggerComponent::TriggerMover called with null IMovableActor"));
		return;
	}
	
	if (Movement == WantsToOpen)
	{
		IMovableActor->SetWantsToOpen();
	}
	else if (Movement == WantsToClose)
	{
		IMovableActor->SetWantsToClose();
	}
}

void UTriggerComponent::OnKeyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor->FindComponentByTag<UStaticMeshComponent>(KeyTag) || OtherActor->ActorHasTag(KeyTag)))
	{
		Movement = WantsToOpen;

		OnPlateTriggered.Broadcast(ETriggerDirection::Open);

		if (bCanPlayMusic)
		{
			OnDoorOpen.Broadcast(MusicToPlay);
		}
	}
	else
	{
		Movement = Idle;
	}
}

void UTriggerComponent::OnKeyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor->FindComponentByTag<UStaticMeshComponent>(KeyTag) || OtherActor->ActorHasTag(KeyTag)))
	{
		Movement = WantsToClose;
		OnPlateTriggered.Broadcast(ETriggerDirection::Close);
	}
	else
	{
		Movement = Idle;
	}
}
