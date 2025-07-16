// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.


#include "CollectionCounterActor.h"

#include "Components/ShapeComponent.h"

#include "GameFramework/Character.h"

ACollectionCounterActor::ACollectionCounterActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// In your actor's constructor
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;
	// Can be overriden in derived classes
	PrimaryActorTick.TickInterval = 1.0f / 30.0f; // 30 Hz = 1/30 seconds per tick

	CollisionComponent = this->GetComponentByClass<UShapeComponent>();
	
	if (CollisionComponent)
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACollectionCounterActor::OnPlayerEnterOverlap);
		CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ACollectionCounterActor::OnPlayerExitOverlap);
	}

}


// Called when the game starts or when spawned
void ACollectionCounterActor::BeginPlay()
{
	Super::BeginPlay();

	CollectiblesAmount = CountCollectables();
	
}


// Called every frame
void ACollectionCounterActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CollectiblesAmount != CountCollectables())
	{
		CollectiblesAmount = CountCollectables();
		OnCollectablesFound.Broadcast(CollectiblesAmount);
	}

	if (CollectiblesAmount == CollectiblesToWin)
	{
		// Makes sure that we're only broadcasting this once
		if (!bDoneOnce)
		{
			OnGameWon.Broadcast();
			bDoneOnce = true;
		}
	}
}


void ACollectionCounterActor::OnPlayerEnterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
	{
		OnPlayerEnteredTrigger.Broadcast();
	}
}


void ACollectionCounterActor::OnPlayerExitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
	{
		OnPlayerExitTrigger.Broadcast();
	}
}


int32 ACollectionCounterActor::CountCollectables() const
{
	TArray<AActor*> OutActors {};
	TSet<AActor*> FittingActors {};
	
	this->GetOverlappingActors(OutActors);

	if (OutActors.Num() == 0)
	{
		return 0;
	}

	for (AActor* Actor : OutActors)
	{
		if (Actor->ActorHasTag(CollectibleTag))
		{
			FittingActors.Add(Actor);
		}
		else if (Actor->FindComponentByTag<UStaticMeshComponent>(CollectibleTag))
		{
			FittingActors.Add(Actor);
		}
	}

	return FittingActors.Num();
}
