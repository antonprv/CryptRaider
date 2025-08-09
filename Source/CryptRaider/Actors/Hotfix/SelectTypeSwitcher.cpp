// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.


#include "SelectTypeSwitcher.h"

#include "GameFramework/Character.h"

void ASelectTypeSwitcher::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ASelectTypeSwitcher::OnPlayerEnterArea);
	OnActorEndOverlap.AddDynamic(this, &ASelectTypeSwitcher::OnPlayerExitArea);
}

void ASelectTypeSwitcher::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	OnActorBeginOverlap.RemoveDynamic(this, &ASelectTypeSwitcher::OnPlayerEnterArea);
	OnActorEndOverlap.RemoveDynamic(this, &ASelectTypeSwitcher::OnPlayerExitArea);
	
	Super::EndPlay(EndPlayReason);
}

void ASelectTypeSwitcher::OnPlayerEnterArea(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
	{
		OnPlayerEnteredArea.Broadcast(false);
	}
}

void ASelectTypeSwitcher::OnPlayerExitArea(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
	{
		OnPlayerEnteredArea.Broadcast(true);
	}
}