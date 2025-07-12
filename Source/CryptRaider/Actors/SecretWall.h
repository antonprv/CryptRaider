// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.

#pragma once

#include "CoreMinimal.h"

#include "CryptRaider/Components/Interfaces/IMovable.h"

#include "GameFramework/Actor.h"
#include "SecretWall.generated.h"

UCLASS()
class CRYPTRAIDER_API ASecretWall : public AActor, public IIMovable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASecretWall();
	
	virtual void SetShouldMove() override;
	virtual void SetShouldNotMove() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnSetShouldMove();
	UFUNCTION(BlueprintImplementableEvent)
	void OnSetShouldNotMove();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMax=0.9f,
		Tooltip="The lower this value, the wider FOV Player needs to actually notice transition"))
	float QuantumEffectPercentage {4.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* SecretDoorMesh {nullptr};

private:
	bool IsPlayerLookingAtDoor() const;
};
