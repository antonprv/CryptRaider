// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.


#include "ScreamerActor.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AScreamerActor::AScreamerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AScreamerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AScreamerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AScreamerActor::SetShouldMove()
{
	if (!IsPlayerLooking())
	{
		PlaySound(OnMoveSound);
		OnSetShouldMove();
	}
}

void AScreamerActor::SetShouldNotMove()
{
	if (!IsPlayerLooking())
	{
		PlaySound(OnExitMoveSound);
		OnSetShouldMove();
	}
}

void AScreamerActor::OnSetShouldMove_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("OnSetShouldMove_Implementation: This function currently does nothing."));
}

void AScreamerActor::OnSetShouldNotMove_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("OnSetShouldNotMove_Implementation: This function currently does nothing."));
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
