// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.


#include "MovableActor.h"

#include "AI/NavigationSystemBase.h"

#include "Components/AudioComponent.h"

#include "CryptRaider/Actors/PressurePlateActor.h"
#include "CryptRaider/Components/MoverComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AMovableActor::AMovableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	RootComponent = StaticMesh;
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Sound Player"));
	AudioComponent->SetupAttachment(StaticMesh);
	MoverComponent = CreateDefaultSubobject<UMoverComponent>(TEXT("Mover Component"));
}

// Called when the game starts or when spawned
void AMovableActor::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APressurePlateActor::StaticClass(), TriggerActors);
	for (AActor* Actor : TriggerActors)
	{
		if (APressurePlateActor* PressurePlateActor = Cast<APressurePlateActor>(Actor))
		{
			PressurePlateActor->OnPPTriggered.AddDynamic(this, &AMovableActor::HandlePressurePlate);
		}
	}
}

void AMovableActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (AActor* Actor : TriggerActors)
	{
		if (APressurePlateActor* PressurePlateActor = Cast<APressurePlateActor>(Actor))
		{
			PressurePlateActor->OnPPTriggered.RemoveDynamic(this, &AMovableActor::HandlePressurePlate);
		}
	}
	
	Super::EndPlay(EndPlayReason);
}

void AMovableActor::HandlePressurePlate(ETriggerDirection TriggerDirection)
{
	DECLARE_LOG_CATEGORY_CLASS(LogAMovableActor, Warning, Warning)
	
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
		UE_LOG(LogAMovableActor, Warning, TEXT("Got Inappropriate value from UTriggerComponent"))
	}
}

void AMovableActor::PlaySound(USoundBase* SoundToPlay)
{
	if (SoundToPlay && GetWorld() && !bPlayedOnce)
	{
		AudioComponent->SetSound(SoundToPlay);
		AudioComponent->Play();
		
		bPlayedOnce = true;
	}
}
