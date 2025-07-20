// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.

#pragma once

#include "CoreMinimal.h"

#include "CryptRaider/Components/Puzzles/TriggerComponent.h"

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
	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> MoveStartSound {nullptr};
	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> MoveEndSound {nullptr};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh {nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URotatorComponent* RotatorComponent {nullptr};

private:
	UPROPERTY()
	UBillboardComponent* EditorBillboard {nullptr};
	
	UPROPERTY()
	UAudioComponent* AudioComponent {nullptr};
	
	UPROPERTY()
	TArray<AActor*> TriggerActors {};
	
	UFUNCTION()
	void HandlePressurePlate(ETriggerDirection TriggerDirection);
	bool bPlayedOnce {false};
	void PlaySound(USoundBase* SoundToPlay);
};
