// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "CollectionCounterActor.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(Blueprintable) )
class CRYPTRAIDER_API ACollectionCounterActor : public ATriggerBox
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ACollectionCounterActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Blueprintable, meta=(Tooltip="Collectible tag to check"))
	FName CollectibleTag {"Gargoyle"};
	UPROPERTY(BlueprintReadOnly)
	int32 CollectiblesAmount {0};

private:

	TArray<AActor*> OverlappingActors {nullptr};
	TSet<AActor*> FittingActors {nullptr};

	int32 CountCollectables();
};
