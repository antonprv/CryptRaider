// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.

#pragma once

#include "CoreMinimal.h"

#include "CryptRaider/GameMusic/MusicHelpers.h"

#include "Engine/TriggerBox.h"

#include "CollectionCounterActor.generated.h"


/**
 * Check how many collectables did the player bring, and if he brough enough, than he won.
 * Also, show collectables UI if we're in the zone and hide if we're not.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameWon, EMusicTriggerType, MusicToPlay);
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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Easter Eggs")
	FName SecretTag {"Labubu"};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CollectionCounter", meta=(Tooltip="Collectible tag to check"))
	FName CollectibleTag {"Gargoyle"};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CollectionCounter", meta=(Tooltip="Win condition"))
	int32 CollectiblesToWin {4};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CollectionCounter | Debug")
	bool bIsDebugEnabled {false};
	
	UPROPERTY(BlueprintAssignable, Category = "Collectables")
	FOnCollectablesFound OnCollectablesFound;

	UPROPERTY(BlueprintAssignable, Category="Collectables")
	FOnPlayerEnteredTrigger OnPlayerEnteredTrigger;
	UPROPERTY(BlueprintAssignable, Category="Collectables")
	FOnPlayerExitTrigger OnPlayerExitTrigger;
	UPROPERTY(BlueprintAssignable, Category="Collectables")
	FOnGameWon OnGameWon;

private:
	// Check if player is in the trigger
	UFUNCTION(NotBlueprintable)
	void OnPlayerEnterOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult);
	
	// Check if player exited the trigger
	UFUNCTION(NotBlueprintable)
	void OnPlayerExitOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex);

	// Check how many collectibles did player bring
	int32 CollectiblesAmount {0};
	UPROPERTY()
	UShapeComponent* CollisionShape {nullptr};
	int32 CountCollectables() const;

	// Win condition logic
	bool bDoneOnce {false};
};
