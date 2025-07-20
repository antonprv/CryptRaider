// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "ScreamerTriggerActor.generated.h"

UENUM()
enum class EScreamerType :uint8
{
	FirstScreamer,
	SecondScreamer
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerEntersScreamerTrigger, EScreamerType, ScreamerType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerExitsScreamerTrigger, EScreamerType, ScreamerType);

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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EScreamerType ScreamerType {EScreamerType::FirstScreamer};
	
	UPROPERTY(BlueprintAssignable, Category="Broadcast screamer trigger")
	FOnPlayerEntersScreamerTrigger OnPlayerEntersScreamerTrigger;
	UPROPERTY(BlueprintAssignable, Category="Broadcast screamer trigger")
	FOnPlayerExitsScreamerTrigger OnPlayerExitsScreamerTrigger;
	
	
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

	UFUNCTION()
	void OnPlayerExitTrigger(
		 UPrimitiveComponent* OverlappedComponent,
		 AActor* OtherActor,
		 UPrimitiveComponent* OtherComp,
		 int32 OtherBodyIndex);
};
