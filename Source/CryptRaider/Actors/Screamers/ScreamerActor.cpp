// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.


#include "ScreamerActor.h"

#include "Components/BillboardComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AScreamerActor::AScreamerActor()
{
	DECLARE_LOG_CATEGORY_CLASS(LogAScreamerActor, Warning, Warning)
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f / 30.0f; // 30 Hz = 1/30 seconds per tick
	

	ScreamerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Screamer Mesh"));
	RootComponent = ScreamerMesh;

	EditorBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Editor Billboard"));

	EditorBillboard->SetupAttachment(ScreamerMesh);
	
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
	if (ScreamerType == EScreamerType::SecondScreamer)
	{
		if (!IsPlayerLooking() && bSecondScreamerDone)
		{
			PlaySound(OnExitMoveSound);
			bCanHideFromPlayer = true;
			SetActorTransform(DefaultTransform);
			OnPlayerEndedScreamer.Broadcast(EMusicTriggerType::Screamer);
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
		PlaySound(OnMoveSound);
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
