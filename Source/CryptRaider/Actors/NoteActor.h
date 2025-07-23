// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interfaces/Pickable.h"

#include "NoteActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerInNoteArea);

class UShapeComponent;
class UStaticMeshComponent;

UCLASS()
class CRYPTRAIDER_API ANoteActor : public AActor, public IPickable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANoteActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (
		Tooltip="Note to show to the player when screamer triggers"))
	UStaticMeshComponent* NoteMesh {nullptr};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (
		Tooltip="Collision to register that player can pick up the note"))
	UShapeComponent* NoteCollisionShape {nullptr};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (
		Tooltip="Sound to play when player picks up the note"))
	UAudioComponent* AudioComponent {nullptr};
	
	UPROPERTY(BlueprintAssignable)
	FOnPlayerInNoteArea OnPlayerInNoteArea;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnPlayerInRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                     int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	virtual void SetPickedUp() override;

};
