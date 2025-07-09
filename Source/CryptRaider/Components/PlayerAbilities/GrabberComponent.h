// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "GrabberComponent.generated.h"


class UCameraComponent;
struct FHitResult;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UGrabberComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabberComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, Category="Grabbing parameters", meta = (Tooltip = "Maximum grabbing distance"))
	float GrabDistance {180.f};
	UPROPERTY(EditAnywhere, Category="Grabbing parameters", meta = (Tooltip = "Radius of the sphere trace for grabbing"))
	float GrabRadius {30.f};
	UPROPERTY(EditAnywhere, Category="Grabbing debug", meta = (Tooltip = "Toggle debug line draw on \\ off"))
	bool bIsDebugEnabled {false};
	
	UFUNCTION(BlueprintCallable, Category= "Physics|Traces", meta = (Keywords = "Trace Line Hit",
		Tooltip="Does a sphere trace from camera view, returns first object hit and hit result, can also draw debug line"))
	void TraceFromCamera (const float& TraceDistance, const float& SphereRadius, FHitResult& OutHitResult, bool& OutIsHit, const bool& bIsDebugging = false);

	UPROPERTY()
	bool bIsValid {true};
	UPROPERTY()
	AActor* OwningActor {nullptr};
	UPROPERTY()
	UCameraComponent* OwnerCamera {nullptr};
	UFUNCTION()
	void DrawDebug(const float& TraceDistance, const float& SphereRadius) const;
};
