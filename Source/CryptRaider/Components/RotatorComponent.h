// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Interfaces/Movable.h"

#include "RotatorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API URotatorComponent : public UActorComponent, public IMovable
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URotatorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Sound indicating opening
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> MoveStartSound {nullptr};
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> MoveEndSound {nullptr};
	
	UPROPERTY(EditAnywhere, Category="Actor Rotation", meta=(Tooltip="Target for rotating back and forth on trigger"))
	FRotator TargetRotation {0.f, 0.f, 0.f};

	UPROPERTY(EditAnywhere)
	float MoveSpeed {0.f};

	UFUNCTION(NotBlueprintable)
	virtual void SetWantsToOpen() override;
	UFUNCTION(NotBlueprintable)
	virtual void SetWantsToClose() override;
	
	UFUNCTION(BlueprintCallable)
	bool GetShouldMove() const;

private:
	UPROPERTY()
	AActor* ActorToMove {nullptr};
	
	FRotator CurrentRotation {0.f, 0.f, 0.f};
	FRotator DefaultRotation {0.f, 0.f, 0.f};
	
	bool bShouldMove {false};
	bool bIsMovingFinished {false};
	
	void RotateActor(const bool& bCanMove, const float& DeltaTimeSeconds);
	bool RotateToRotation(const FRotator& End, const float & DeltaTimeSeconds);
	void PlaySound(USoundBase* SoundToPlay);
};
