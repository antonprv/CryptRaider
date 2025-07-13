// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.

#pragma once

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"

#include "CryptRaider/Components/Interfaces/Movable.h"


#include "TriggerComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTriggerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, Category="Debug", meta = (Tooltip="Log out in console if Key is in lock if set to true"))
	bool bIsDebugEnabled {false};
	UPROPERTY(EditAnywhere, Category="Object Detection", meta = (Tooltip = "Set actor or component tag to look for"))
	FName KeyTag {"Key"};
	
	UPROPERTY(BlueprintReadOnly, Category="Object Detection",
		meta = (Tooltip = "All actors (or components) with the KeyTag"))
	TArray<AActor*> KeyActors {nullptr};
	
	UFUNCTION(BlueprintCallable, meta=(MustImplement="IIMovable"))
	void TriggerMover (TScriptInterface<IMovable> IMovableActor) const;

private:
	bool bDoneOnce {false};
	UPROPERTY()
	AActor* KeyActor {nullptr};
	bool bWantsToTrigger {false};
	
	AActor* GetFittingActor(TArray<AActor*>& OverlappingActors) const;
	void DebugShowFitActor(const AActor* Key) const;
	void DebugShowFitActorWithComponent(const AActor* Key) const;
};