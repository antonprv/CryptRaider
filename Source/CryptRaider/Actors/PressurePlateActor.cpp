// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.


#include "PressurePlateActor.h"

#include "Components/BillboardComponent.h"

// Sets default values
APressurePlateActor::APressurePlateActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

#if WITH_EDITORONLY_DATA
	DECLARE_LOG_CATEGORY_CLASS(LogAPressurePlateActor, Warning, Warning)
#endif
	
	TriggerComponent = CreateDefaultSubobject<UTriggerComponent>(TEXT("Trigger Component"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pressure Plate Mesh"));
	RootComponent = StaticMesh;
	TriggerComponent->SetupAttachment(StaticMesh);

	TriggerComponent->OnDoorOpen.AddDynamic(this, &APressurePlateActor::HandleDoorOpen);
	TriggerComponent->OnPlateTriggered.AddDynamic(this, &APressurePlateActor::HandlePPTriggered);

#if WITH_EDITORONLY_DATA
	EditorBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Editor Billboard"));

	EditorBillboard->SetupAttachment(StaticMesh);
	
	static ConstructorHelpers::FObjectFinder<UTexture2D> SpriteFinder(
		TEXT("/Engine/EditorResources/EmptyActor"));

	if (SpriteFinder.Succeeded())
	{
		EditorBillboard->SetSprite(SpriteFinder.Object);
	}
	else if (!SpriteFinder.Succeeded())
	{
		UE_LOG(LogAPressurePlateActor, Warning,
			TEXT("Was not able to find the EditorBillboard sprite"))
	}
#endif
	
}

// Called when the game starts or when spawned
void APressurePlateActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void APressurePlateActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	TriggerComponent->OnDoorOpen.RemoveDynamic(this, &APressurePlateActor::HandleDoorOpen);
	TriggerComponent->OnPlateTriggered.RemoveDynamic(this, &APressurePlateActor::HandlePPTriggered);
	
	Super::EndPlay(EndPlayReason);
}

void APressurePlateActor::HandleDoorOpen(const EMusicTriggerType MusicToPlay)
{
	OnDungeonDoorOpen.Broadcast(MusicToPlay);
}

void APressurePlateActor::HandlePPTriggered(ETriggerDirection TriggerDirection)
{
	OnPPTriggered.Broadcast(TriggerDirection);
}

