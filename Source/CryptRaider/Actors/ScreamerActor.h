// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.

#pragma once

#include "CoreMinimal.h"

#include "CryptRaider/Components/Interfaces/Movable.h"

#include "GameFramework/Actor.h"
#include "ScreamerActor.generated.h"

UCLASS( ClassGroup=(Custom), meta=(Blueprintable) )
class CRYPTRAIDER_API AScreamerActor : public AActor, public IMovable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScreamerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quantum Effect", meta=(ClampMax=0.9,
		Tooltip="To what extend are things happening when player is not looking, 0 - they never happen, 0.9 - they can happen right before player's eyes"))
	float QuantumEffectPercentage {0.4};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound Effect", meta=(
		Tooltip="Sound to play when actor triggers"))
	USoundBase* OnMoveSound {nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound Effect", meta=(
		Tooltip="Sound to play when actor stops triggering"))
	USoundBase* OnExitMoveSound {nullptr};

	
	UFUNCTION(NotBlueprintable)
	virtual void SetShouldMove() override;
	UFUNCTION(NotBlueprintable)
	virtual void SetShouldNotMove() override;

	UFUNCTION(BlueprintNativeEvent)
	void OnSetShouldMove();
	UFUNCTION(BlueprintNativeEvent)
	void OnSetShouldNotMove();

	
private:
	bool IsPlayerLooking() const;
	void PlaySound(USoundBase* SoundToPlay) const;
};
