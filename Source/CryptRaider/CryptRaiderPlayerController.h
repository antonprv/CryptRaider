// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "GlobalInterfaces/CollectionOwner.h"

#include "CryptRaiderPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CRYPTRAIDER_API ACryptRaiderPlayerController : public APlayerController, public ICollectionOwner
{
	GENERATED_BODY()
	
};
