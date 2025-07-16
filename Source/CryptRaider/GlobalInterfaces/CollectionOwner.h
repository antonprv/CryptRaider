// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CollectionOwner.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UCollectionOwner : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CRYPTRAIDER_API ICollectionOwner
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void ShowCollectionWidget();
	UFUNCTION(BlueprintNativeEvent)
	void HideCollectionWidget();

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
