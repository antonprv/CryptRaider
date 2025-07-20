// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.

#pragma once

#include "CoreMinimal.h"

#include "CryptRaider/Components/Puzzles/TriggerComponent.h"
#include "CryptRaider/GameMusic/MusicHelpers.h"

#include "GameFramework/Actor.h"
#include "PressurePlateActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDungeonDoorOpen, EMusicTriggerType, MusicToPlay);

UCLASS()
class CRYPTRAIDER_API APressurePlateActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APressurePlateActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnDungeonDoorOpen OnDungeonDoorOpen;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTriggerComponent* TriggerComponent {nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh {nullptr};

private:
	UFUNCTION()
	void HandleDoorOpen(EMusicTriggerType& MusicToPlay);
};
