// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "ScreamerTriggerActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerEnterFirstTrigger);

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(Blueprintable) )
class CRYPTRAIDER_API AScreamerTriggerActor : public ATriggerBox
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScreamerTriggerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintAssignable, Category="Broadcast screamer trigger")
	FOnPlayerEnterFirstTrigger OnPlayerEnterFirstTrigger;
	
	
private:
	UPROPERTY()
	UShapeComponent* CollisionShape {nullptr};
	
	UFUNCTION()
	void OnPlayerEnterTrigger(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult & SweepResult);
};
