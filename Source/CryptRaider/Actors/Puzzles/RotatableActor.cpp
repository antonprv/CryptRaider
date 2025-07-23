// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.


#include "RotatableActor.h"

#include "Components/AudioComponent.h"
#include "Components/BillboardComponent.h"

#include "CryptRaider/Actors/PressurePlateActor.h"
#include "CryptRaider/Components/RotatorComponent.h"
#include "CryptRaider/GameSave/CryptRaiderSave.h"
#include "CryptRaider/GameSave/Interfaces/GameSaver.h"

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
	AudioComponent->SetAutoActivate(false);
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
		PlaySound(MoveStartSound, RoomSoundType);
		break;
	case ETriggerDirection::Close:
		bPlayedOnce = false;
		PlaySound(MoveEndSound, RoomSoundType);
		break;
	default:
		UE_LOG(LogARotatableActor, Warning, TEXT("Got Inappropriate value from UTriggerComponent"))
	}
}

void ARotatableActor::PlaySound(USoundBase* SoundToPlay, const ERoomSoundType& RoomSound)
{
	DECLARE_LOG_CATEGORY_CLASS(LogARotatableActor, Warning, Warning)
	
	if (SoundToPlay && GetWorld() && !bPlayedOnce && AudioComponent)
	{
		AudioComponent->SetSound(SoundToPlay);
		SetSoundVolume();
		AudioComponent->Play();
		switch (RoomSound)
		{
		case ERoomSoundType::SmallRoom:
			AudioComponent->SetTriggerParameter(USoundHelpers::SmallRoomTriggerName);
			break;
		case ERoomSoundType::BigRoom:
			AudioComponent->SetTriggerParameter(USoundHelpers::BigRoomTriggerName);
			break;
		default:
			UE_LOG(LogARotatableActor, Warning, TEXT("PlaySound function triggered non-existing value"))
		}
		
		bPlayedOnce = true;
	}
}


void ARotatableActor::SetSoundVolume() const
{
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance->Implements<UGameSaver>())
	{
		if (IGameSaver* GameSaver = Cast<IGameSaver>(GameInstance))
		{
			if (const UCryptRaiderSave* SavedGame = GameSaver->GetGameData())
			{
				AudioComponent->SetVolumeMultiplier(SavedGame->PlayerSave.EnvironmentVolume);
			}
		}
	}
}

