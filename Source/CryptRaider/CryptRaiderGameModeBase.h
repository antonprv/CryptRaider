// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CryptRaiderGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class CRYPTRAIDER_API ACryptRaiderGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
    ACryptRaiderGameModeBase();
	
protected:
    virtual void BeginPlay() override;
    
private:
    static void SetLowScalability();
    static void SetMaxFPS(int32 MaxFPS);
    static void SetScreenPercentage(int32 ScreenPercentage);
    	
};
