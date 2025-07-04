// Fill out your copyright notice in the Description page of Project Settings.


#include "CryptRaiderGameModeBase.h"


#include "GameFramework/GameUserSettings.h"

ACryptRaiderGameModeBase::ACryptRaiderGameModeBase()
{
}

void ACryptRaiderGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	SetLowScalability();
	SetMaxFPS(100);
	SetScreenPercentage(100);
}

void ACryptRaiderGameModeBase::SetLowScalability()
{
	if (GEngine)
	{
		if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
		{
			// Set all scalability settings to low (0)
			Settings->ScalabilityQuality.AntiAliasingQuality = 0;
			Settings->ScalabilityQuality.EffectsQuality = 0;
			Settings->ScalabilityQuality.PostProcessQuality = 1;
			Settings->ScalabilityQuality.ShadowQuality = 2;
			Settings->ScalabilityQuality.TextureQuality = 0;
			Settings->ScalabilityQuality.ViewDistanceQuality = 0;

			// Apply and save the settings
			Settings->ApplyNonResolutionSettings();
			Settings->SaveSettings();
		}
	}
}

void ACryptRaiderGameModeBase::SetMaxFPS(const int32 MaxFPS)
{
	if (GEngine)
	{
		GEngine->SetMaxFPS(MaxFPS);
	}
}

void ACryptRaiderGameModeBase::SetScreenPercentage(const int32 ScreenPercentage)
{
	if (GEngine && GEngine->GameViewport)
	{
		// Set the screen percentage (resolution fraction) via console variable
		FString Command = FString::Printf(TEXT("r.ScreenPercentage %d"), ScreenPercentage);
		GEngine->GameViewport->Exec(nullptr, *Command, *GLog);
	}
}