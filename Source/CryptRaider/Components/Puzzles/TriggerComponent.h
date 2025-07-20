// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.

#pragma once

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"

#include "CryptRaider/Components/Interfaces/Movable.h"
#include "CryptRaider/GameMusic/MusicHelpers.h"


#include "TriggerComponent.generated.h"

UENUM()
enum ETriggerDirection
{
	Open,
	Close
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDoorOpen, EMusicTriggerType, MusicToPlay);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlateTriggered, ETriggerDirection, TriggerDirection);

UENUM()
enum EMovement : uint8
{
	WantsToOpen,
	WantsToClose,
	Idle
};

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTriggerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	UPROPERTY(EditAnywhere, Category="Music")
	bool bCanPlayMusic {false};
	
	UPROPERTY(EditAnywhere, Category="Debug", meta = (Tooltip="Log out in console if Key is in lock if set to true"))
	bool bIsDebugEnabled {false};
	UPROPERTY(EditAnywhere, Category="Object Detection", meta = (Tooltip = "Set actor or component tag to look for"))
	FName KeyTag {"Key"};
	
	UPROPERTY(BlueprintReadOnly, Category="Object Detection",
		meta = (Tooltip = "All actors (or components) with the KeyTag"))
	TArray<AActor*> KeyActors {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Music")
	EMusicTriggerType MusicToPlay {EMusicTriggerType::DungeonDoorOpen};
	UPROPERTY(BlueprintAssignable, Category="Music")
	FOnDoorOpen OnDoorOpen;
	UPROPERTY(BlueprintAssignable, Category="Sound")
	FOnPlateTriggered OnPlateTriggered;
	
	UFUNCTION(BlueprintCallable, meta=(MustImplement="IMovable"))
	void TriggerMover (TScriptInterface<IMovable> IMovableActor) const;

private:
	bool bDoneOnce {false};
	UPROPERTY()
	AActor* KeyActor {nullptr};
	EMovement Movement {Idle};
	
	UFUNCTION()
	void OnKeyBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult & SweepResult);
	UFUNCTION()
	void OnKeyEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
};