// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.


#include "TriggerBoxComponent.h"

// Sets default values for this component's properties
UTriggerBoxComponent::UTriggerBoxComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}

void UTriggerBoxComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTriggerBoxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
