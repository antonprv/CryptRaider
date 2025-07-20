// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.


#include "PressurePlateActor.h"

// Sets default values
APressurePlateActor::APressurePlateActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TriggerComponent = CreateDefaultSubobject<UTriggerComponent>(TEXT("Trigger Component"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pressure Plate Mesh"));
	RootComponent = StaticMesh;
	TriggerComponent->SetupAttachment(StaticMesh);

	TriggerComponent->OnDoorOpen.AddDynamic(this, &APressurePlateActor::HandleDoorOpen);

}

// Called when the game starts or when spawned
void APressurePlateActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void APressurePlateActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	TriggerComponent->OnDoorOpen.RemoveDynamic(this, &APressurePlateActor::HandleDoorOpen);
	
	Super::EndPlay(EndPlayReason);
}

void APressurePlateActor::HandleDoorOpen(EMusicTriggerType& MusicToPlay)
{
	OnDungeonDoorOpen.Broadcast(MusicToPlay);
}

