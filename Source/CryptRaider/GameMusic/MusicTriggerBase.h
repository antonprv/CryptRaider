// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.

#pragma once

#include "CoreMinimal.h"

#include "CryptRaider/GameMusic/MusicHelpers.h"

#include "Engine/TriggerBox.h"

#include "MusicTriggerBase.generated.h"

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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable, Category="Music Collision")
	FOnPlayerEnterMusicArea OnPlayerEnterMusicArea;  
	UPROPERTY(BlueprintAssignable, Category="Music Collision")
	FOnPlayerExitMusicArea OnPlayerExitMusicArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MusicCollision")
	EMusicTriggerType MusicToPlay {EMusicTriggerType::Entrance};
	
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
