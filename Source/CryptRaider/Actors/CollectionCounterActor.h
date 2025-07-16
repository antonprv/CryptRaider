// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.

#pragma once

#include "CoreMinimal.h"

#include "Engine/TriggerBox.h"

#include "CollectionCounterActor.generated.h"


/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerEnteredTrigger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerExitTrigger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCollectablesFound, int32, CollectableAmount);


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CollectionCounter", meta=(Tooltip="Collectible tag to check"))
	FName CollectibleTag {"Gargoyle"};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CollectionCounter | Debug")
	bool bIsDebugEnabled {false};

	UPROPERTY(BlueprintAssignable, Category = "Collectables")
	FOnCollectablesFound OnCollectablesFound;

	UPROPERTY(BlueprintAssignable, Category="Collectables")
	FOnPlayerEnteredTrigger OnPlayerEnteredTrigger;
	UPROPERTY(BlueprintAssignable, Category="Collectables")
	FOnPlayerExitTrigger OnPlayerExitTrigger;

private:
	UFUNCTION(NotBlueprintable)
	void OnPlayerEnterOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult);
	
	UFUNCTION(NotBlueprintable)
	void OnPlayerExitOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex);
	
	int32 CollectiblesAmount {0};
	UShapeComponent* CollisionComponent {nullptr};
	int32 CountCollectables() const;
};
