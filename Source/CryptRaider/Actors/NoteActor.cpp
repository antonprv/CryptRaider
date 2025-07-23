// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.


#include "NoteActor.h"

#include "Components/AudioComponent.h"
#include "Components/ShapeComponent.h"

#include "GameFramework/Character.h"

// Sets default values
ANoteActor::ANoteActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	NoteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Note Mesh"));
	RootComponent = NoteMesh;
	NoteCollisionShape = CreateDefaultSubobject<UShapeComponent>(TEXT("Note pickup collision"));
	NoteCollisionShape->SetupAttachment(NoteMesh);

	NoteCollisionShape->SetGenerateOverlapEvents(true);
	
	FCollisionResponseContainer CollisionResponse;
	CollisionResponse.Pawn = ECR_Overlap;
	NoteCollisionShape->SetCollisionResponseToChannels(CollisionResponse);
	
	NoteCollisionShape->OnComponentBeginOverlap.AddDynamic(this, &ANoteActor::OnPlayerInRange);
}

// Called when the game starts or when spawned
void ANoteActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANoteActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	NoteCollisionShape->OnComponentBeginOverlap.RemoveDynamic(this, &ANoteActor::OnPlayerInRange);
	
	Super::EndPlay(EndPlayReason);
}

void ANoteActor::OnPlayerInRange(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult & SweepResult)
{
	if (OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
	{
		OnPlayerInNoteArea.Broadcast();
	}
}

void ANoteActor::SetPickedUp()
{
	IPickable::SetPickedUp();

	NoteMesh->SetVisibility(false);
	AudioComponent->Play();
}

