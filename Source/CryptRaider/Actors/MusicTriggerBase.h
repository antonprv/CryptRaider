// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "MusicTriggerBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerEnterMusicArea);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerExitMusicArea);

UENUM(BlueprintType, Category="Music")
enum class EMusicTriggerType : uint8
{
	Entrance UMETA(DisplayName = "Entrance Theme"),
	DungeonDoorOpen UMETA(DisplayName = "Dungeon Door Open Theme"),
	SecretRoom UMETA(DisplayName = "Secret Room Theme"),
	ChaseSequence UMETA(DisplayName = "Chase Sequence Theme"),
	ScreamerTheme UMETA(DisplayName = "")
};

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(Blueprintable) )
class CRYPTRAIDER_API AMusicTriggerBase : public ATriggerBox
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AMusicTriggerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintCallable, Category="Music Collision")
	FOnPlayerEnterMusicArea OnPlayerEnterMusicArea;
	UPROPERTY(BlueprintCallable, Category="Music Collision")
	FOnPlayerExitMusicArea OnPlayerExitMusicArea;
	
private:
	UFUNCTION()
	void OnPlayerEnterOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerExitOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UPROPERTY()
	UShapeComponent* CollisionShape {nullptr};
};
