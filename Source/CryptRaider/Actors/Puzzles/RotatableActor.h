// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.

#pragma once

#include "CoreMinimal.h"

#include "CryptRaider/Components/Puzzles/TriggerComponent.h"
#include "CryptRaider/GameSound/SoundHelpers.h"

#include "GameFramework/Actor.h"
#include "RotatableActor.generated.h"

class URotatorComponent;
class UAudioComponent;
class UBillboardComponent;



UCLASS()
class CRYPTRAIDER_API ARotatableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotatableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Sound indicating opening
	UPROPERTY(EditAnywhere, Category = "Sound", meta=(
		Tooltip="WARNING! Expects a MetaSound, otherwise echo will be unavailable"))
	TObjectPtr<USoundBase> MoveStartSound {nullptr};
	UPROPERTY(EditAnywhere, Category = "Sound", meta=(
		Tooltip="WARNING! Expects a MetaSound, otherwise echo will be unavailable"))
	TObjectPtr<USoundBase> MoveEndSound {nullptr};

	UPROPERTY(EditAnywhere, Category="Sound", meta=(
		Tooltip="BigRoom = echo, SmallRoom = no echo"))
	ERoomSoundType RoomSoundType {ERoomSoundType::SmallRoom};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* StaticMesh {nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAudioComponent* AudioComponent {nullptr};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	URotatorComponent* RotatorComponent {nullptr};

private:
	UPROPERTY()
	UBillboardComponent* EditorBillboard {nullptr};
	
	UPROPERTY()
	TArray<AActor*> TriggerActors {};
	
	UFUNCTION()
	void HandlePressurePlate(ETriggerDirection TriggerDirection);
	bool bPlayedOnce {false};
	void PlaySound(USoundBase* SoundToPlay, const ERoomSoundType& RoomSound);
};
