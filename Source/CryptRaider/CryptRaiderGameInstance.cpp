// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.


#include "CryptRaiderGameInstance.h"

#include "GameSave/CryptRaiderSave.h"
#include "GameSave/SaveHelpers.h"

#include "Kismet/GameplayStatics.h"


void UCryptRaiderGameInstance::Init()
{
	Super::Init();
	
	CryptRaiderSave = LoadPlayerSave();
}


void UCryptRaiderGameInstance::LoadGameData()
{
	CryptRaiderSave = LoadPlayerSave();
}


void UCryptRaiderGameInstance::SaveGameData(const bool& bIsAsync)
{
	if (!bIsAsync)
	{
		UGameplayStatics::SaveGameToSlot(
			CryptRaiderSave,
			USaveHelpers::DefaultSlotName,
			USaveHelpers::DefaultSlotIndex);
	}
	else if (bIsAsync)
	{
		// Bind the async save completion delegate
		FAsyncSaveGameToSlotDelegate AsyncDelegate;
		AsyncDelegate.BindUObject(this, &UCryptRaiderGameInstance::HandleAsyncSaveCompleted);
		
		// Execute async save
		UGameplayStatics::AsyncSaveGameToSlot(
			CryptRaiderSave,
			USaveHelpers::DefaultSlotName,
			USaveHelpers::DefaultSlotIndex,
			AsyncDelegate);
	}
}


void UCryptRaiderGameInstance::HandleAsyncSaveCompleted(
	const FString& SlotName, int SlotIndex, const bool bWasSaveSuccessful)
{
	DECLARE_LOG_CATEGORY_CLASS(LogIGameSaver, Display, Display)
	
	if (bWasSaveSuccessful)
	{
		UE_LOG(LogIGameSaver, Log, TEXT("Save successful to slot %s"), *SlotName);
	}
	else
	{
		UE_LOG(LogIGameSaver, Warning, TEXT("Failed to save to slot %s"), *SlotName);
	}
}


UCryptRaiderSave* UCryptRaiderGameInstance::GetGameData()
{
	return CryptRaiderSave;
}


// We always want to save data in async way to avoid game stutters if it is from trigger
void UCryptRaiderGameInstance::SaveGameDataFromTrigger(FPlayerSave& PlayerSave)
{
	CryptRaiderSave->PlayerSave = PlayerSave;
	SaveGameData(true);
}

// Tries to load player save. Creates default one if fails
UCryptRaiderSave* UCryptRaiderGameInstance::LoadPlayerSave()
{
	if (UGameplayStatics::DoesSaveGameExist(
		USaveHelpers::DefaultSlotName,
		USaveHelpers::DefaultSlotIndex))
	{
		USaveGame* LoadedSave = UGameplayStatics::LoadGameFromSlot(
			USaveHelpers::DefaultSlotName,
			USaveHelpers::DefaultSlotIndex);

		if (UCryptRaiderSave* PlayerSave = Cast<UCryptRaiderSave>(LoadedSave))
		{
			return PlayerSave;
		}
		return CreateDefaultSave();
	}
	return CreateDefaultSave();
}

// Creates default save file with all volume set to 1
UCryptRaiderSave* UCryptRaiderGameInstance::CreateDefaultSave()
{
	USaveGame* SaveGame {nullptr};
	SaveGame = UGameplayStatics::CreateSaveGameObject(UCryptRaiderSave::StaticClass());
	if (UCryptRaiderSave* PlayerSave = Cast<UCryptRaiderSave>(SaveGame))
	{
		return PlayerSave;
	}
	return nullptr;
}
