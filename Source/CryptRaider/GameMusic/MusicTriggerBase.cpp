// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.


#include "MusicTriggerBase.h"

#include "Components/ShapeComponent.h"

#include "GameFramework/Character.h"


AMusicTriggerBase::AMusicTriggerBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// In your actor's constructor
	
	CollisionShape = GetComponentByClass<UShapeComponent>();

	if (CollisionShape)
	{
		CollisionShape->OnComponentBeginOverlap.AddDynamic(this, &AMusicTriggerBase::OnPlayerEnterOverlap);
		CollisionShape->OnComponentEndOverlap.AddDynamic(this, &AMusicTriggerBase::OnPlayerExitOverlap);
	}
}

void AMusicTriggerBase::BeginPlay()
{
	Super::BeginPlay();
}

void AMusicTriggerBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CollisionShape->OnComponentBeginOverlap.RemoveDynamic(this, &AMusicTriggerBase::OnPlayerEnterOverlap);
	CollisionShape->OnComponentEndOverlap.RemoveDynamic(this, &AMusicTriggerBase::OnPlayerExitOverlap);
	OnPlayerEnterMusicArea.Clear();
	OnPlayerExitMusicArea.Clear();
	
	Super::EndPlay(EndPlayReason);
}


void AMusicTriggerBase::OnPlayerEnterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// avoid playing entrance music at the start of the game instead of main menu music (temporary solution)
	if (bIsFirstLaunch && MusicToPlay == EMusicTriggerType::Entrance)
	{
		bIsFirstLaunch = false;
	}
	else if (!bIsFirstLaunch)
	{
		if (OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
		{
			OnPlayerEnterMusicArea.Broadcast(MusicToPlay);
		}
	}
}

void AMusicTriggerBase::OnPlayerExitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->IsA(ACharacter::StaticClass()))
	{
		OnPlayerExitMusicArea.Broadcast(MusicToPlay);
	}
}


