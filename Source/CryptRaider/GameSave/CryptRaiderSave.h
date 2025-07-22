// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.

#pragma once

#include "CoreMinimal.h"
#include "SaveHelpers.h"

#include "GameFramework/SaveGame.h"
#include "CryptRaiderSave.generated.h"


/**
 * 
 */
UCLASS(Blueprintable)
class CRYPTRAIDER_API UCryptRaiderSave : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FPlayerSave PlayerSave {};
	
};

