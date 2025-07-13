// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Movable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMovable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CRYPTRAIDER_API IMovable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(NotBlueprintable)
	virtual void SetShouldMove() = 0;
	UFUNCTION(NotBlueprintable)
	virtual void SetShouldNotMove() = 0;
};
