// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "Interfaces/IMovable.h"

#include "MoverComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UMoverComponent : public UActorComponent, public IIMovable
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
	UPROPERTY(EditAnywhere, Category="Actor Rotation", meta=(Tooltip="Target for rotating back and forth on trigger"))
	FRotator TargetRotation {0.f, 0.f, 0.f};

	UPROPERTY(EditAnywhere)
	float MoveSpeed {0.f};
	UPROPERTY(EditAnywhere)
	float Acceleration {0.f};
	UPROPERTY(EditAnywhere)
	bool bShouldMove {false};
	
	UFUNCTION(NotBlueprintable)
	virtual void SetShouldMove() override;

private:
	UPROPERTY()
	AActor* ActorToMove {nullptr};
	UPROPERTY()
	FVector CurrentLocation {0.f, 0.f, 0.f};
	UPROPERTY()
	FVector DefaultLocation {0.f, 0.f, 0.f};

	UPROPERTY()
	FRotator CurrentRotation {0.f, 0.f, 0.f};
	UPROPERTY()
	FRotator DefaultRotation {0.f, 0.f, 0.f};

	UPROPERTY()
	bool bIsMovingFinished {false};
	UPROPERTY()
	UWorld* GameWorld {nullptr};
	
	UFUNCTION(NotBlueprintable)
	void MoveActor(const bool& bCanMove, const float & DeltaTimeSeconds);
	UFUNCTION(NotBlueprintable)
	void RotateActor(const bool& bCanMove, const float & DeltaTimeSeconds);

	UFUNCTION(NotBlueprintable)
	bool MoveToLocation (FVector& Start, const FVector& End, const float & DeltaTimeSeconds);
	UFUNCTION(NotBlueprintable)
	bool RotateToRotation (FRotator& Start, const FRotator& End, const float & DeltaTimeSeconds);
};
