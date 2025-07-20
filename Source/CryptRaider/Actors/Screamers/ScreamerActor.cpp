// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.


#include "ScreamerActor.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AScreamerActor::AScreamerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ScreamerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Screamer Mesh"));
	RootComponent = ScreamerMesh;
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


void AScreamerActor::HandlePlayerEnterScreamer(EScreamerType ScreamerType)
{
	DECLARE_LOG_CATEGORY_CLASS(LogAScreamerActor, Warning, Warning)
	
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
	if (!IsPlayerLooking() && bSecondScreamerDone)
	{
		PlaySound(OnExitMoveSound);
		ScreamerMesh->SetVisibility(false);
		SetActorTransform(DefaultTransform);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(SecondScreamerTimer);
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


void AScreamerActor::PerformFirstScreamer() const
{
	if (!IsPlayerLooking())
	{
		ScreamerMesh->SetVisibility(false);
	}
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
	SetActorTransform(ScreamerTransform);
	if (!IsPlayerLooking())
	{
		PlaySound(OnMoveSound);
		ScreamerMesh->SetVisibility(true);
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
    
	// Calculate direction to door
	const FVector DoorLocation = this->GetActorLocation();
	const FVector ToDoor = (DoorLocation - CameraLocation).GetSafeNormal();
	const FVector LookDirection = CameraRotation.Vector();

	// Check angle threshold (45 degrees)
	float DotProduct = FVector::DotProduct(LookDirection, ToDoor);
	
	return DotProduct > QuantumEffectPercentage;
}

void AScreamerActor::PlaySound(USoundBase* SoundToPlay) const
{
	if (SoundToPlay && GetWorld())
	{
		UGameplayStatics::PlaySoundAtLocation(
			this, 
			SoundToPlay, 
			GetOwner()->GetActorLocation() // Play at the owner's location
		);
	}
}
