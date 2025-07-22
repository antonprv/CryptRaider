// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "GameSave/GameSaver.h"

#include "CryptRaiderGameInstance.generated.h"

class UCryptRaiderSave;
/**
 * 
 */
UCLASS()
class CRYPTRAIDER_API UCryptRaiderGameInstance : public UGameInstance, public IGameSaver
{
	GENERATED_BODY()

public:
	UFUNCTION(Blueprintable)
	virtual void LoadGameData() override;
	void HandleAsyncSaveCompleted(const FString& SlotName, int SlotIndex, bool bWasSaveSuccessful);
	UFUNCTION(Blueprintable)
	virtual void SaveGameData(const bool& bIsAsync) override;
	UFUNCTION(Blueprintable)
	virtual UCryptRaiderSave* GetGameData() override;
	UFUNCTION(Blueprintable)
	virtual void SaveGameDataFromTrigger(FPlayerSave& PlayerSave) override;
	
protected:
	virtual void Init() override;

private:
	UPROPERTY()
	UCryptRaiderSave* CryptRaiderSave;

	static UCryptRaiderSave* LoadPlayerSave();
	static UCryptRaiderSave* CreateDefaultSave();
};
