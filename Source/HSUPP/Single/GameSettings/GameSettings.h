// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSUPP.h"
#include "UObject/NoExportTypes.h"
#include "GameSettings.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API UGameSettings : public UObject
{
	GENERATED_BODY()

private:
	 class UGameUserSettings* GameSetting;
	 
public:
	void InitGameSetting(class UGameUserSettings* gameSetting);

	void SetScreenResolution(FIntPoint newScreenResolution);

	void SetScreenMode(EWindowMode::Type windowMode);

	EWindowMode::Type GetScreenMode();
	FString GetScreenModeToString();
	FIntPoint GetScreenResolution();

	FIntPoint GetFullScreenResolution();
};
