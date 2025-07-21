// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "Interfaces/Movable.h"

#include "Puzzles/TriggerComponent.h"

#include "MoverComponent.generated.h"

class UAudioComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UMoverComponent : public UActorComponent, public IMovable
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMoverComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, Category="Actor Movement", meta=(Tooltip="Target for moving back and forth on trigger"))
	FVector TargetLocation {0.f, 0.f, 0.f};
	
	UPROPERTY(EditAnywhere)
	float OpenSpeed {0.f};
	UPROPERTY(EditAnywhere)
    float CloseSpeed {0.f};

	UFUNCTION(NotBlueprintable)
	virtual void SetWantsToOpen() override;
	UFUNCTION(NotBlueprintable)
	virtual void SetWantsToClose() override;
	
	UFUNCTION(BlueprintCallable)
	bool GetShouldMove() const;

private:
	UPROPERTY()
	AActor* ActorToMove {nullptr};
	
	FVector CurrentLocation {0.f, 0.f, 0.f};
	FVector DefaultLocation {0.f, 0.f, 0.f};
	
	bool bShouldMove {false};
	bool bIsMovingFinished {false};
	
	void MoveActor(const bool& bCanMove, const float & DeltaTimeSeconds);
	bool MoveToLocation (const FVector& End, const float & DeltaTimeSeconds, const float & InterpSpeed);
};
