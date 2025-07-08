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
	UPROPERTY()
	float GrabDistabce {2.f};
	UFUNCTION(BlueprintCallable, Category= "Physics|Traces", meta = (Keywords = "Trace Line Hit"))
	void TraceGrabLineFromCameraView (float TraceDistance, FHitResult & OutHitResult, bool & OutIsHit) const;
	
	UPROPERTY()
	AActor* OwningActor {nullptr};
	UPROPERTY()
	UCameraComponent* OwnerCamera {nullptr};
};
