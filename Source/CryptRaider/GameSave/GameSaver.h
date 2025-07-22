// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameSaver.generated.h"

struct FPlayerSave;
class UCryptRaiderSave;
// This class does not need to be modified.
UINTERFACE(MinimalAPI, NotBlueprintable)
class UGameSaver : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CRYPTRAIDER_API IGameSaver
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable)
	virtual void LoadGameData() = 0;
	UFUNCTION(BlueprintCallable)
	virtual void SaveGameData(const bool & bIsAsync) = 0;
	UFUNCTION(BlueprintCallable)
	virtual UCryptRaiderSave* GetGameData() = 0;
	UFUNCTION(BlueprintCallable)
	virtual void SaveGameDataFromTrigger(UPARAM(ref) FPlayerSave & PlayerSave) = 0;
};
