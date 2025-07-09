// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.
#pragma once

#include "CoreMinimal.h"
#include "BlueprintEditor.h"

#include "Components/ActorComponent.h"
#include "GrabberComponent.generated.h"


class UPhysicsHandleComponent;
class UCameraComponent;
struct FHitResult;
class UPhysicsHandleComponent;

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

	UPROPERTY(EditAnywhere, Category="Grabbing parameters", meta = (Tooltip = "Maximum grabbing distance"))
	float GrabDistance {180.f};
	UPROPERTY(EditAnywhere, Category="Grabbing parameters", meta = (Tooltip = "Radius of the sphere trace for grabbing"))
	float GrabRadius {10.f};
	UPROPERTY(EditAnywhere, Category="Grabbing debug", meta = (Tooltip = "Toggle debug line draw on \\ off"))
	bool bIsDebugEnabled {false};

	UPROPERTY(BlueprintReadOnly, Category = "Grab trace result", meta = (Tooltip = "True if grabbing succeded"))
	bool bHasHit {false};
	UPROPERTY(BlueprintReadOnly, Category = "Grab trace result",
		meta = (Tooltip = "Hit Result of Sphere Trace from camera, it is never empty, use bool Has hit for checking"))
	FHitResult GrabResult;

	UFUNCTION(BlueprintCallable, Category= "Physics|Traces", meta = (Keywords = "Sweep Sphere Grab",
		Tooltip="Does a sphere trace from camera view, returns first object hit and hit result, can also draw debug line"))
	void TraceFromCamera (const float& TraceDistance, const float& SphereRadius,
		FVector& OutStartTrace, FVector& OutEndTrace,
		FHitResult& OutHitResult, bool& OutIsHit,
		const bool& bIsDebugging = false);

	UFUNCTION(BlueprintCallable, Category = "Physics|Grab", meta = (Keywords = "Input Grab",
		Tooltip = "Signals to grab object using physics handle"))
	void Grab();

	UFUNCTION(BlueprintCallable, Category = "Physics|Grab", meta = (Keywords = "Input Grab",
		Tooltip = "Signals to release object using physics handle"))
	void Release();
	
private:
	UPROPERTY()
	bool bIsValid {true};
	UPROPERTY()
	AActor* OwningActor {nullptr};
	UPROPERTY()
	UCameraComponent* OwnerCamera {nullptr};
	UPROPERTY()
	UPhysicsHandleComponent* GrabHandle {nullptr};
	UPROPERTY()
	FVector GrabStart {0.f, 0.f, 0.f};
	UPROPERTY()
	FVector GrabEnd {0.f, 0.f, 0.f};
	UPROPERTY()
	bool bIsGrabbing {false};
	
	UFUNCTION()
	void KeepGrabbing() const;
	UFUNCTION()
	void DrawDebug(const float& TraceDistance,  const float& SphereRadius,
		const FVector& StartTrace, const FVector& EndTrace);
};
