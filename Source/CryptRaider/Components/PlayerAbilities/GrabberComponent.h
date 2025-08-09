// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.
#pragma once

#include "CoreMinimal.h"

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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

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
	
	

	UFUNCTION(BlueprintCallable, Category = "Physics|Grab", meta = (Keywords = "Input Grab",
		Tooltip = "Signals to grab object using physics handle"))
	void Grab();

	UFUNCTION(BlueprintCallable, Category = "Physics|Grab", meta = (Keywords = "Input Grab",
		Tooltip = "Signals to release object using physics handle"))
	void Release();
	
private:
	bool bIsValid {true};
	UPROPERTY()
	AActor* OwningActor {nullptr};
	UPROPERTY()
	UCameraComponent* OwnerCamera {nullptr};
	UPROPERTY()
	UPhysicsHandleComponent* GrabHandle {nullptr};
	FVector GrabStart {0.f, 0.f, 0.f};
	FVector GrabEnd {0.f, 0.f, 0.f};
	bool bIsGrabbing {false};
	
	bool bCanTraceTroughWalls { true };
	UPROPERTY()
	TArray<AActor*> TypeSwitchActors {};
	void SubscribeToTypeSwitchers();
	UFUNCTION()
	void SetCanTraceTroughWalls(bool bCanTrace);

	void TraceFromCamera (const float& TraceDistance, const float& SphereRadius,
	                      FVector& OutStartTrace, FVector& OutEndTrace,
	                      FHitResult& OutHitResult, bool& OutIsHit,
	                      const bool& bIsDebugging = false);
	
	void KeepGrabbing() const;

#if WITH_EDITORONLY_DATA
	void DrawDebug(const float& TraceDistance,  const float& SphereRadius,
		const FVector& StartTrace, const FVector& EndTrace);
#endif
	
};
