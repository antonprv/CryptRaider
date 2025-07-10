// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.


#include "TriggerComponent.h"


// Sets default values for this component's properties
UTriggerComponent::UTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}

void UTriggerComponent::BeginPlay()
{
	DECLARE_LOG_CATEGORY_CLASS(UTriggerBoxComponentLog, Display, Display)
	
	Super::BeginPlay();

	
	UE_LOG(UTriggerBoxComponentLog, Display, TEXT("%s on %s is ready!"), *GetName(), *GetOwner()->GetName())
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
