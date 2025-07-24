// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.


#include "ScreamerActor.h"

#include "Components/AudioComponent.h"
#include "Components/BillboardComponent.h"

#include "CryptRaider/GameSave/CryptRaiderSave.h"
#include "CryptRaider/GameSave/Interfaces/GameSaver.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AScreamerActor::AScreamerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f / 30.0f; // 30 Hz = 1/30 seconds per tick
	

	ScreamerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Screamer Mesh"));
	RootComponent = ScreamerMesh;

	EditorBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Editor Billboard"));

	EditorBillboard->SetupAttachment(ScreamerMesh);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioComponent->SetupAttachment(ScreamerMesh);
	AudioComponent->SetAutoActivate(false);

	
	static ConstructorHelpers::FObjectFinder<UTexture2D> SpriteFinder(
		TEXT("/Engine/EditorResources/EmptyActor"));

	if (SpriteFinder.Succeeded())
	{
		EditorBillboard->SetSprite(SpriteFinder.Object);
	}
	else if (!SpriteFinder.Succeeded())
	{
		UE_LOG(LogAScreamerActor, Warning,
			TEXT("Was not able to find the EditorBillboard sprite"))
	}
}


// Called when the game starts or when spawned
void AScreamerActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetWorld())
	{
		DefaultTransform = GetActorTransform();
		
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AScreamerTriggerActor::StaticClass(), ScreamerTriggers);

		for (AActor* Actor : ScreamerTriggers)
		{
			if (AScreamerTriggerActor* TriggerActor = Cast<AScreamerTriggerActor>(Actor))
			{
				TriggerActor->OnPlayerEntersScreamerTrigger.AddDynamic(
					this,
					&AScreamerActor::HandlePlayerEnterScreamer);
				TriggerActor->OnPlayerExitsScreamerTrigger.AddDynamic(
					this,
					&AScreamerActor::HandlePlayerExitScreamer);
			}
		}
	}
}


void AScreamerActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnsubscribeFromAll();
	OnPlayerStartedScreamer.Clear();
	Super::EndPlay(EndPlayReason);
}

void AScreamerActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	HideFromPlayer();
}


void AScreamerActor::HandlePlayerEnterScreamer(EScreamerType ScreamerType)
{
	switch (ScreamerType)
	{
	case EScreamerType::FirstScreamer:
		PerformFirstScreamer();
		break;
	case EScreamerType::SecondScreamer:
		PerformSecondScreamer();
		break;
		default:
			UE_LOG(LogAScreamerActor, Warning,
				TEXT("Received neither Second, nor First screamer, but still triggered"))
		break;
	}
}

void AScreamerActor::HandlePlayerExitScreamer(EScreamerType ScreamerType)
{
	if (ScreamerType == EScreamerType::SecondScreamer)
	{
		OnPlayerEndedScreamer.Broadcast(EMusicTriggerType::Screamer);
		if (!IsPlayerLooking() && bSecondScreamerDone)
		{
			PlaySound(RoomSoundType);
			bCanHideFromPlayer = true;
			SetActorTransform(DefaultTransform);
			OnScreamerDone.Broadcast();
		}
		else
		{
			bCanPlaySecondScreamer = false;
			bCanHideFromPlayer = true;
			GetWorld()->GetTimerManager().ClearTimer(SecondScreamerTimer);
		}
	}
}


void AScreamerActor::UnsubscribeFromAll()
{
	for (AActor* Actor : ScreamerTriggers)
	{
		if (AScreamerTriggerActor* TriggerActor = Cast<AScreamerTriggerActor>(Actor))
		{
			TriggerActor->OnPlayerEntersScreamerTrigger.RemoveDynamic(
				this,
				&AScreamerActor::HandlePlayerEnterScreamer);
			TriggerActor->OnPlayerExitsScreamerTrigger.RemoveDynamic(
				this,
				&AScreamerActor::HandlePlayerExitScreamer);
		}
	}
}


void AScreamerActor::HideFromPlayer() const
{
	if (bCanHideFromPlayer && !IsPlayerLooking())
	{
		ScreamerMesh->SetVisibility(false);
	}
	else if (!bCanHideFromPlayer)
	{
		ScreamerMesh->SetVisibility(true);
	}
}

void AScreamerActor::PerformFirstScreamer()
{
	bCanHideFromPlayer = true;
}

void AScreamerActor::PerformSecondScreamer()
{
	GetWorld()->GetTimerManager().SetTimer(
		SecondScreamerTimer,
		this,
		&AScreamerActor::ExecuteSecondScreamer,
		SecondScreamerDelay);
}

void AScreamerActor::ExecuteSecondScreamer()
{
	if (!IsPlayerLooking() && bCanPlaySecondScreamer)
	{
		SetActorTransform(ScreamerTransform);
		PlaySound(RoomSoundType);
		bCanHideFromPlayer = false;
		OnPlayerStartedScreamer.Broadcast(EMusicTriggerType::Screamer);

		bSecondScreamerDone = true;
	}
}

bool AScreamerActor::IsPlayerLooking() const
{
	const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return false;
    
	// Get player's viewpoint
	FVector CameraLocation;
	FRotator CameraRotation;
	PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
    
	// Calculate direction to LookPlace
	const FVector ScreamerLocation = this->GetActorLocation();
	const FVector ToDoor = (ScreamerLocation - CameraLocation).GetSafeNormal();
	const FVector LookDirection = CameraRotation.Vector();

	// Check angle threshold (45 degrees)
	float DotProduct = FVector::DotProduct(LookDirection, ToDoor);
	
	return DotProduct > QuantumEffectPercentage;
}


void AScreamerActor::PlaySound(const ERoomSoundType& RoomSound)
{
	if (GetWorld() && !bPlayedOnce && AudioComponent)
	{
		AudioComponent->Play();
		SetSoundVolume();
		switch (RoomSound)
		{
		case ERoomSoundType::SmallRoom:
			AudioComponent->SetTriggerParameter(USoundHelpers::SmallRoomTriggerName);
			break;
		case ERoomSoundType::BigRoom:
			AudioComponent->SetTriggerParameter(USoundHelpers::BigRoomTriggerName);
			break;
		default:
			UE_LOG(LogAScreamerActor, Warning, TEXT("PlaySound function triggered non-existing value"))
		}
		
		bPlayedOnce = true;
	}
}

void AScreamerActor::SetSoundVolume() const
{
	UGameInstance* GameInstance = GetGameInstance();
	if (IGameSaver* GameSaver = Cast<IGameSaver>(GameInstance))
	{
		if (const UCryptRaiderSave* SavedGame = GameSaver->GetGameData())
		{
			AudioComponent->SetVolumeMultiplier(SavedGame->PlayerSave.EnvironmentVolume);
		}
	}
	else
	{
		UE_LOG(LogAScreamerActor, Error, TEXT("Failed to get volume from user settings"))
	}
}
