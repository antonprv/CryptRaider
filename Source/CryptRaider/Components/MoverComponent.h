// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoverComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UMoverComponent : public UActorComponent
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

private:
	UPROPERTY(EditAnywhere)
	FVector TargetLocation {0.f, 0.f, 0.f};
	UPROPERTY(EditAnywhere)
	float MoveSpeed {0.f};
	UPROPERTY(EditAnywhere)
	bool bShouldMove {false};

	UPROPERTY()
	AActor* ActorToMove {nullptr};
	UPROPERTY()
	FVector CurrentLocation {0.f, 0.f, 0.f};
	UPROPERTY()
	bool bIsMovingFinished {false};

	UFUNCTION()
	void MoveActor(const bool& bCanMove);
		
};
