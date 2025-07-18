// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MusicHelpers.generated.h"

/**
 * Just a collection of delegate types and enum types.
 * Unreal Engine wil only allow this in an empty UObject file,
 * so this whole thing is a workaround
 */

/*
 * Types of music that we can play based on player entering or exiting a specific area or other triggers
 */
UENUM(BlueprintType, Category="Music")
enum class EMusicTriggerType : uint8
{
	Entrance UMETA(DisplayName = "Entrance Theme"),
	DungeonDoorOpen UMETA(DisplayName = "Dungeon Door Open Theme"),
	CryptLoop UMETA(DisplayName = "Crypt Loop Theme"),
	SecretRoom UMETA(DisplayName = "Secret Room Theme"),
	ChaseSequence UMETA(DisplayName = "Chase Sequence Theme"),
	Screamer UMETA(DisplayName = "Screamer Theme"),
	EndCreditsNormal UMETA(DisplayName = "End Credits Normal"),
	EndCreditsSecret UMETA(DisplayName = "End Credits Secret")
};

/*
 * Delegate types that will allow us to broadcast to MusicActor, asking it to play or stop specific tracks.
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerEnterMusicArea, EMusicTriggerType, MusicType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerExitMusicArea, EMusicTriggerType, MusicType);

// This should remain empty
UCLASS()
class CRYPTRAIDER_API UMusicHelpers : public UObject
{
	GENERATED_BODY()
	
};
