// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.

#pragma once

#include "CoreMinimal.h"

#include "CryptRaider/Components/Puzzles/TriggerComponent.h"
#include "CryptRaider/GameMusic/MusicHelpers.h"

#if WITH_EDITOR
#include "Components/BillboardComponent.h"
#endif

#include "GameFramework/Actor.h"
#include "PressurePlateActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDungeonDoorOpen, EMusicTriggerType, MusicToPlay);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPPTriggered, ETriggerDirection, TriggerDirection);


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
	UPROPERTY(BlueprintAssignable)
	FOnPPTriggered OnPPTriggered;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTriggerComponent* TriggerComponent {nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh {nullptr};

private:
	
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	UBillboardComponent* EditorBillboard {nullptr};
#endif
	
	UFUNCTION()
	void HandleDoorOpen(const EMusicTriggerType MusicToPlay);
	UFUNCTION()
	void HandlePPTriggered(ETriggerDirection TriggerDirection);
};
