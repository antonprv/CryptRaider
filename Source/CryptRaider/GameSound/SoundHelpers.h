// Made by Antosh (anton.prv), derivative work strictly prohibited, except for non-commercial use.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SoundHelpers.generated.h"

/**
 * Just a collection of enum types.
 * Unreal Engine wil only allow this in an empty UObject file,
 * so this whole thing is a workaround
 */

/**
 * Works closely with Metasounds, triggers sound variation based on room size
 */

UENUM()
enum class ERoomSoundType
{
	BigRoom,
	SmallRoom
};

UCLASS()
class CRYPTRAIDER_API USoundHelpers : public UObject
{
	GENERATED_BODY()

public:
	static const FName SmallRoomTriggerName;
	static const FName BigRoomTriggerName;
};
