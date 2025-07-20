// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.


#include "RotatableActor.h"

#include "Components/AudioComponent.h"
#include "Components/BillboardComponent.h"

#include "CryptRaider/Actors/PressurePlateActor.h"
#include "CryptRaider/Components/RotatorComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ARotatableActor::ARotatableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DECLARE_LOG_CATEGORY_CLASS(LogARotatableActor, Warning, Warning)
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	RootComponent = StaticMesh;
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Sound Player"));
	AudioComponent->SetupAttachment(StaticMesh);
	RotatorComponent = CreateDefaultSubobject<URotatorComponent>(TEXT("Rotator Component"));

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
		UE_LOG(LogARotatableActor, Warning,
			TEXT("Was not able to find the EditorBillboard sprite"))
	}
}

// Called when the game starts or when spawned
void ARotatableActor::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APressurePlateActor::StaticClass(), TriggerActors);
	for (AActor* Actor : TriggerActors)
	{
		if (APressurePlateActor* PressurePlateActor = Cast<APressurePlateActor>(Actor))
		{
			PressurePlateActor->OnPPTriggered.AddDynamic(this, &ARotatableActor::HandlePressurePlate);
		}
	}
	
}

void ARotatableActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (AActor* Actor : TriggerActors)
	{
		if (APressurePlateActor* PressurePlateActor = Cast<APressurePlateActor>(Actor))
		{
			PressurePlateActor->OnPPTriggered.RemoveDynamic(this, &ARotatableActor::HandlePressurePlate);
		}
	}
	
	Super::EndPlay(EndPlayReason);
}

void ARotatableActor::HandlePressurePlate(ETriggerDirection TriggerDirection)
{
	DECLARE_LOG_CATEGORY_CLASS(LogARotatableActor, Warning, Warning)
	
	switch (TriggerDirection)
	{
	case ETriggerDirection::Open:
		bPlayedOnce = false;
		PlaySound(MoveStartSound);
		break;
	case ETriggerDirection::Close:
		bPlayedOnce = false;
		PlaySound(MoveEndSound);
		break;
	default:
		UE_LOG(LogARotatableActor, Warning, TEXT("Got Inappropriate value from UTriggerComponent"))
	}
}

void ARotatableActor::PlaySound(USoundBase* SoundToPlay)
{
	if (SoundToPlay && GetWorld() && !bPlayedOnce)
	{
		AudioComponent->SetSound(SoundToPlay);
		AudioComponent->Play();
		
		bPlayedOnce = true;
	}
}



