// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.

#pragma once

#include "CoreMinimal.h"
#include "TaskSyncManager.h"

#include "CryptRaider/GameMusic/MusicHelpers.h"

#include "GameFramework/Actor.h"

#include "Interfaces/Pickable.h"

#include "NoteActor.generated.h"

class UTextRenderComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerInNoteArea, bool, bIsInNoteArea);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerPickedUpNote);

class USphereComponent;

UCLASS()
class CRYPTRAIDER_API ANoteActor : public AActor, public IPickable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANoteActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void CleanUp();

	UFUNCTION()
	void OnNoteBecomeVisible(EMusicTriggerType MusicToPlay);
	UFUNCTION()
	void OnNoteBecomeInvisible();
	
	UFUNCTION()
	void OnPlayerEnterPickupArea(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult);
	UFUNCTION()
	void OnPlayerExitPickupArea(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	virtual void SetPickedUp() override;

	UPROPERTY(BlueprintAssignable, Category="Pickup events")
	FOnPlayerPickedUpNote OnPlayerPickedUpNote;
	UPROPERTY(BlueprintAssignable, Category="Pickup events")
	FOnPlayerInNoteArea OnPlayerInNoteArea;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup Collision")
	USphereComponent* CollisionSphere {nullptr};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup Model")
	UStaticMeshComponent* NoteMesh {nullptr};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category ="Pickup advice")
	UTextRenderComponent* FloatingText {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Note Widget")
	UClass* WidgetClass {nullptr};

private:
	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> ScreamerActors;
	bool bCanPlayerSeeNote {false};

	UPROPERTY()
	TWeakObjectPtr<APlayerController> PlayerController {nullptr};

	bool bIsWidgetCreated {false};
	void BindInteractionInput();
	void UnbindInteractionInput();
	void OnInteractPressed();
};
