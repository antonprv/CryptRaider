// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.


#include "TriggerComponent.h"

#include "UObject/FastReferenceCollector.h"


// Sets default values for this component's properties
UTriggerComponent::UTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Super::SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// Set default collision responses
	
	FCollisionResponseContainer ResponseContainer;
	ResponseContainer.GameTraceChannel2 = ECR_Overlap;
	ResponseContainer.PhysicsBody = ECR_Overlap;
	ResponseContainer.Pawn = ECR_Overlap;
	Super::SetCollisionResponseToChannels(ResponseContainer);
	
	
	SetGenerateOverlapEvents(true);
	
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
// 	this->GetOverlappingActors(OverlappingActors);
// 	this->GetOverlappingComponents(OverlappingComponents);
//
// 	for (const AActor* Element : OverlappingActors)
// 	{
// 		UE_LOG(LogTemp, Display, TEXT("%s"), *Element->GetName())
// 	}
// 	
// 	for (const UPrimitiveComponent* Element : OverlappingComponents)
// 	{
// 		UE_LOG(LogTemp, Display, TEXT("%s"), *Element->GetName())
// 	}
}
