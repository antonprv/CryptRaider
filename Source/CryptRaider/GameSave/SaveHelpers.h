// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SaveHelpers.generated.h"

/**
 * 
 */
UCLASS()
class CRYPTRAIDER_API USaveHelpers : public UObject
{
	GENERATED_BODY()

public:
	// Is always 1.f
	static const float DefaultVolumeValue;
	static const FString DefaultSlotName;
	static const int32 DefaultSlotIndex;
	
};

USTRUCT(BlueprintType)
struct FPlayerSave
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	float MusicVolume {USaveHelpers::DefaultVolumeValue};
	UPROPERTY(BlueprintReadWrite)
	float PlayerVolume {USaveHelpers::DefaultVolumeValue};
	UPROPERTY(BlueprintReadWrite)
	float EnvironmentVolume {USaveHelpers::DefaultVolumeValue};
	
};

