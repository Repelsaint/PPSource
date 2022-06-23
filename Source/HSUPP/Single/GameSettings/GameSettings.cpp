// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSettings.h"
#include "Engine/Engine.h"
#include "GameFramework/GameUserSettings.h"
#include "GameMode/HGameMode.h"



void UGameSettings::InitGameSetting(UGameUserSettings* gameSetting)
{
	// 게임세팅을 지정합니다.
	GameSetting = gameSetting;

}

void UGameSettings::SetScreenResolution(FIntPoint newScreenResolution)
{
	// 아직 게임세팅이 지정되기 전이라면 실행시키지 않습니다.
	if(GameSetting == nullptr)  return;

	// 현재 스크린해상도를 변경하고 적용합니다.
	GameSetting->SetScreenResolution(newScreenResolution);
	GameSetting->ApplySettings(false);
}

void UGameSettings::SetScreenMode(EWindowMode::Type windowMode)
{
	// 아직 게임세팅이 지정되기 전이라면 실행시키지 않습니다.
	if (GameSetting == nullptr)  return;

	// 현재 스크린모드를 변경하고 적용합니다.
	GameSetting->SetFullscreenMode(windowMode);
	GameSetting->ApplySettings(false);

	
}

EWindowMode::Type UGameSettings::GetScreenMode()
{
	// 현재 스크린 모드를 불러서 반환합니다.
	return GameSetting->GetFullscreenMode();
}

FString UGameSettings::GetScreenModeToString()
{
	//현재 스크린모드를 불러와서 FString형식으로 변환해서반환합니다.
	switch (GetScreenMode())
	{
	case EWindowMode::Fullscreen:
		return TEXT("전체모드");
		break;
	case EWindowMode::WindowedFullscreen:
		return TEXT("전체창모드");
		break;
	case EWindowMode::Windowed:
		return TEXT("창모드");
		break;
	case EWindowMode::NumWindowModes:
		return TEXT("전체모드");
		break;
	default:
		return TEXT("창모드");
		break;
	}
}


FIntPoint UGameSettings::GetScreenResolution()
{
	return GameSetting->GetScreenResolution();
}

FIntPoint UGameSettings::GetFullScreenResolution()
{
	return GameSetting->GetDesktopResolution();
}
