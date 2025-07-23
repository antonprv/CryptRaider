// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.

#pragma once

#include "CoreMinimal.h"
#include "ScreamerTriggerActor.h"

#include "CryptRaider/GameMusic/MusicHelpers.h"

#include "GameFramework/Actor.h"
#include "ScreamerActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStartedScreamer, EMusicTriggerType, MusicToPlay);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerEndedScreamer, EMusicTriggerType, MusicToPlay);

class UBillboardComponent;
class ANoteActor;
class UAudioComponent;

UCLASS( ClassGroup=(Custom), meta=(Blueprintable) )
class CRYPTRAIDER_API AScreamerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScreamerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quantum Effect", meta=(ClampMax=0.9,
		Tooltip="To what extend are things happening when player is not looking, 0 - they never happen, 0.9 - they can happen right before player's eyes"))
	float QuantumEffectPercentage {0.1};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SecondScreamerDelay {0.2f};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (
		Tooltip="In-game mesh of the screamer actor"))
	UStaticMeshComponent* ScreamerMesh {nullptr};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(
		Tooltip = "Note that screamer Actor is holding when player sees it"))
	ANoteActor* NoteActor {nullptr};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (
		ToolTip="Where actor is goint to move and how is it goint to rotate on second screamer"))
	FTransform ScreamerTransform {GetActorTransform()};

	// Sounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound Effect", meta=(
		Tooltip="Sound to play when actor triggers"))
	USoundBase* OnMoveSound {nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound Effect", meta=(
		Tooltip="Sound to play when actor stops triggering"))
	USoundBase* OnExitMoveSound {nullptr};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAudioComponent* AudioComponent {nullptr};
	
	// Music
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Music")
	FOnPlayerStartedScreamer OnPlayerStartedScreamer;
	bool bSecondScreamerDone {false};
	UPROPERTY(BlueprintAssignable, Category="Music");
	FOnPlayerEndedScreamer OnPlayerEndedScreamer;

private:
	UPROPERTY()
	FTimerHandle SecondScreamerTimer {};
	
	UPROPERTY()
	TArray<AActor*> ScreamerTriggers {};

	UPROPERTY()
	FTransform DefaultTransform {};

	UPROPERTY()
	UBillboardComponent* EditorBillboard {nullptr};

	bool bCanPlaySecondScreamer {true};
	
	UFUNCTION()
	void HandlePlayerEnterScreamer(EScreamerType ScreamerType);
	UFUNCTION()
	void HandlePlayerExitScreamer(EScreamerType ScreamerType);

	void UnsubscribeFromAll();

	bool bCanHideFromPlayer {false};
	void HideFromPlayer() const;

	void PerformFirstScreamer();
	void PerformSecondScreamer();

	void ExecuteSecondScreamer();
	bool IsPlayerLooking() const;
	void PlaySound(USoundBase* SoundToPlay) const;
	void SetSoundVolume() const;
};
