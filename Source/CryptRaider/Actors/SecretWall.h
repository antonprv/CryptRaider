// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.

#pragma once

#include "CoreMinimal.h"

#include "CryptRaider/Components/Interfaces/Movable.h"

#include "GameFramework/Actor.h"
#include "SecretWall.generated.h"

UCLASS( ClassGroup=(Custom), meta=(Blueprintable) )
class CRYPTRAIDER_API ASecretWall : public AActor, public IMovable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASecretWall();

	UFUNCTION(NotBlueprintable)
	virtual void SetWantsToOpen() override;
	UFUNCTION(NotBlueprintable)
	virtual void SetWantsToClose() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, Category="Secret Wall")
	void OnSetShouldMove();
	UFUNCTION(BlueprintNativeEvent, Category="Secret Wall")
	void OnSetShouldNotMove();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> MoveStartSound {nullptr};
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> MoveEndSound {nullptr};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMax=0.9f,
		Tooltip="The lower this value, the wider FOV Player needs to actually notice transition"))
	float QuantumEffectPercentage {0.4f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* SecretDoorMesh {nullptr};

private:
	bool IsPlayerLookingAtDoor() const;
	void PlaySound(USoundBase* SoundToPlay);
};
