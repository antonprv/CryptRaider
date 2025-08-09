// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "SelectTypeSwitcher.generated.h"

/**
 * Whether to always trace for objects to grab or to stop player from doing that if he's near wall.
 * Fires that off based on event.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerEnteredArea, bool, bCanCastThroughWalls);

UCLASS()
class CRYPTRAIDER_API ASelectTypeSwitcher : public ATriggerBox
{
	GENERATED_BODY()

public:
	FOnPlayerEnteredArea OnPlayerEnteredArea {};
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
	void OnPlayerEnterArea(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
	void OnPlayerExitArea(AActor* OverlappedActor, AActor* OtherActor);
};
