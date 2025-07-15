// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.


#include "CollectionCounterActor.h"

ACollectionCounterActor::ACollectionCounterActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// In your actor's constructor
	PrimaryActorTick.bCanEverTick = true;
	// Can be overriden in derived classes
	PrimaryActorTick.TickInterval = 1.0f / 30.0f; // 30 Hz = 1/30 seconds per tick


}

// Called when the game starts or when spawned
void ACollectionCounterActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollectionCounterActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CollectiblesAmount = CountCollectables();

}

int32 ACollectionCounterActor::CountCollectables()
{
	TArray<AActor*> OutActors {nullptr};
	TSet<AActor*> FittingActors {nullptr};
	
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
	}

	return FittingActors.Num();
}
