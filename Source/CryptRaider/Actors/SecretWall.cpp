// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.


#include "SecretWall.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ASecretWall::ASecretWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SecretDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SecretDoorMesh"));
	RootComponent = SecretDoorMesh;
	SecretDoorMesh->SetGenerateOverlapEvents(true);
}


// Called when the game starts or when spawned
void ASecretWall::BeginPlay()
{
	Super::BeginPlay();
}

void ASecretWall::OnSetShouldMove_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("OnSetShouldMove is not implemented"))
}

void ASecretWall::OnSetShouldNotMove_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("OnSetShouldNotMove is not implemented"))
}


void ASecretWall::SetWantsToOpen()
{
	PlaySound(MoveStartSound);
	if (!IsPlayerLookingAtDoor())
	{
		SecretDoorMesh->SetVisibility(false);
		SecretDoorMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
		OnSetShouldMove();
	}
}

void ASecretWall::SetWantsToClose()
{
	PlaySound(MoveEndSound);
	if (!IsPlayerLookingAtDoor())
	{
		SecretDoorMesh->SetVisibility(true);
		SecretDoorMesh->SetCollisionResponseToAllChannels(ECR_Block);
		OnSetShouldNotMove();
	}
}


bool ASecretWall::IsPlayerLookingAtDoor() const
{
	const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return false;
    
	// Get player's viewpoint
	FVector CameraLocation;
	FRotator CameraRotation;
	PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
    
	// Calculate direction to door
	const FVector DoorLocation = SecretDoorMesh->GetComponentLocation();
	const FVector ToDoor = (DoorLocation - CameraLocation).GetSafeNormal();
	const FVector LookDirection = CameraRotation.Vector();

	// Check angle threshold (45 degrees)
	float DotProduct = FVector::DotProduct(LookDirection, ToDoor);
	
	return DotProduct > QuantumEffectPercentage;
}

void ASecretWall::PlaySound(USoundBase* SoundToPlay)
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