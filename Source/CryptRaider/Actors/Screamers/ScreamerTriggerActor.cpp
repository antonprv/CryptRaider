// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.


#include "ScreamerTriggerActor.h"

#include "Components/ShapeComponent.h"

#include "GameFramework/Character.h"

AScreamerTriggerActor::AScreamerTriggerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// In your actor's constructor
	PrimaryActorTick.bCanEverTick = false;

	CollisionShape = GetComponentByClass<UShapeComponent>();
	CollisionShape->OnComponentBeginOverlap.AddDynamic(this, &AScreamerTriggerActor::OnPlayerEnterTrigger);
}

// Called when the game starts or when spawned
void AScreamerTriggerActor::BeginPlay()
{
	Super::BeginPlay();
}

void AScreamerTriggerActor::OnPlayerEnterTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
	{
		OnPlayerEnterFirstTrigger.Broadcast();
	}
}
