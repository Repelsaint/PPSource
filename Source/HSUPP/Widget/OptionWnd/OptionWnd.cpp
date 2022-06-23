// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionWnd.h"

#include "Widget/MainWidget.h"
#include "Widget/InteractableMessage/InteractableMessage.h"

#include "GameMode/HGameMode.h"

#include "Single/GameSettings/GameSettings.h"

#include "Engine/Engine.h"

#include "Actor/DefaultLevelActor/DefaultLevelActor.h"



void UOptionWnd::OnResolutionSizeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{	
	// FString으로 들어온 해상도 사이즈를 X 값과 Y값을 나누기 위한 변수입니다.
	bool bXsizeFin = false;

	// 들어온 값의 X값과 Y값을 저장할 변수입니다.
	float changedxSize = 0.0f;
	float changedySize = 0.0f;

	for (int i = 0; i < SelectedItem.Len(); ++i)
	{
		// FString의 변수에 한 글자를 집어넣습니다.
		FString currentInt;
		currentInt += SelectedItem[i];
		
		// 들어온 문자가 * 이라면
		if (SelectedItem[i] == TEXT('*'))
		{
			// X값의 처리가 끝났음을 알립니다.
			bXsizeFin = true;
		}
		// *이 아니면서 X값의 처리가 끝났다면
		else if (bXsizeFin)
		{
			// Y값에 값을 추가해줍니다.
			// 새로운 숫자를 제대로 입력하기위해 기존의 값을 10 곱하여 더해줍니다. 
			changedySize *= 10;
			changedySize += FCString::Atoi(*currentInt);
		}
		else
		{
			//bool 값이 false 라면 아직 x 사이즈값이 다 들어오지 않은것이므로
			// x사이즈 값에 추가해줍니다.
			changedxSize *= 10;
			changedxSize += FCString::Atoi(*currentInt);
		}
	}

	// 입력된 값이 제대로 들어와있는지 검사합니다.
	if (changedxSize != 0 && changedySize != 0)
	{
		// 해상도를 변경하고 적용해줍니다.
		GetHInstance()->GetGameSetting()->SetScreenResolution(FIntPoint(changedxSize, changedySize));
		GetHInstance()->GetMainWidget()->OnScreenOptionChanged();
	}

}

void UOptionWnd::OnScreenSizeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{

	//FString으로 들어온 값을 검사하여 윈도우 모드를 적용해줍니다.
	if (SelectedItem == TEXT("창모드"))
	{
		GetHInstance()->GetGameSetting()->SetScreenMode(EWindowMode::Windowed);
	}
	else if (SelectedItem == TEXT("전체창모드"))
	{
		GetHInstance()->GetGameSetting()->SetScreenMode(EWindowMode::WindowedFullscreen);
	}
	else if (SelectedItem == TEXT("전체화면"))
	{
		GetHInstance()->GetGameSetting()->SetScreenMode(EWindowMode::Fullscreen);
	}

	// 모드변경에 따른 해상도의 변경을 처리해줍니다.
	GetHInstance()->GetMainWidget()->OnScreenOptionChanged();
}

UOptionWnd::UOptionWnd(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// 기본 창 크기를 지정합니다.
	WndSize = FVector2D(900.0f, 450.0f);
}

void UOptionWnd::NativeConstruct()
{
	Super::NativeConstruct();

	// 옵션창을 초기화 합니다.
	InitializeOptionWnd();
}

void UOptionWnd::InitializeOptionWnd()
{
	// 타이틀의 제목을 옵션으로 변경합니다.
	SetTitleText(TEXT("Option"));

	// 레벨의 타입을 검사합니다.
	if (Cast<ADefaultLevelActor>(GetWorld()->GetLevelScriptActor())->GetLevelType() == ELevelActorType::Home)
	{
		// 홈레벨에서 소환을 했다면 홈 / 종료 버튼을 숨깁니다.
		Button_Home->SetVisibility(ESlateVisibility::Collapsed);
		Button_Exit->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	// 버튼클릭시 호출될 메서드를 바인딩합니다.
	Button_Sound->OnClicked.AddDynamic(this, &UOptionWnd::OnSoundButtonClicked);
	Button_Option->OnClicked.AddDynamic(this, &UOptionWnd::OnOptionButtonClicked);
	Button_Home->OnClicked.AddDynamic(this, &UOptionWnd::OnHomeButtonClicked);
	Button_Exit->OnClicked.AddDynamic(this, &UOptionWnd::OnExitButtonClicked);

#pragma region SetDefaultComboBox

	// 현재 스크린사이즈를 읽어옵니다.
	FIntPoint selectedScreen = GetHInstance()->GetGameSetting()->GetScreenResolution();

	//스크린 x사이즈를 FString 형식으로 변환하고 그뒤에 *와 Y사이즈를 집어넣어 FString으로의 형변환을 완성시킵니다.
	FString selectedScreenString = FString::FromInt(selectedScreen.X);
	selectedScreenString += TEXT("*");
	selectedScreenString += FString::FromInt(selectedScreen.Y);

	// 완성된 FString 형식으로 콤보박스의 선택된옵션을 지정해줍니다.
	ComboBox_Resolution->SetSelectedOption(selectedScreenString);

	// 현재 스크린 모드를 FString형식으로 받아와서 콤보박스의 옵션을 지정해줍니다.
	ComboBox_WndMode->SetSelectedOption(GetHInstance()->GetGameSetting()->GetScreenModeToString());

	// 콤보박스가 바뀔때 호출될 메서드를 바인딩합니다.
	ComboBox_Resolution->OnSelectionChanged.AddDynamic(this, &UOptionWnd::OnResolutionSizeChanged);
	ComboBox_WndMode->OnSelectionChanged.AddDynamic(this, &UOptionWnd::OnScreenSizeChanged);

#pragma endregion

#pragma region SetSoundValue

	//마스터 사운드 클래스를 게임인스턴스에서 불러옵니다.
	USoundClass* masterSClass = GetHInstance()->GetMasterSoundClass();

	// 사운드값과 음소거값을 불러옵니다.
	TArray<float> allPrevSoundValue = GetHInstance()->GetAllSoundValues();
	TArray<bool> allMuteStates = GetHInstance()->GetAllMuteStates();

	// 각볼륨 블루프린트에 적용되야하는 타입을 넣어주고 텍스트와 볼륨 음소거값을 지정합니다.
	BP_Option_MVolume->SetVolumeType(masterSClass->Properties.Volume);
	BP_Option_MVolume->SetPrevVolumeSize(allPrevSoundValue[0]);
	BP_Option_MVolume->SetMutable(allMuteStates[0]);
	BP_Option_MVolume->RefreshSizeText();


	BP_Option_BackVolume->SetBarName(TEXT("배경 볼륨"));
	BP_Option_BackVolume->SetVolumeType(masterSClass->ChildClasses[0]->Properties.Volume);
	BP_Option_BackVolume->SetPrevVolumeSize(allPrevSoundValue[1]);
	BP_Option_BackVolume->SetMutable(allMuteStates[1]);
	BP_Option_BackVolume->RefreshSizeText();


	BP_Option_ChaVolume->SetBarName(FString(TEXT("캐릭터 볼륨")));
	BP_Option_ChaVolume->SetVolumeType(masterSClass->ChildClasses[1]->Properties.Volume);
	BP_Option_ChaVolume->SetPrevVolumeSize(allPrevSoundValue[2]);
	BP_Option_ChaVolume->SetMutable(allMuteStates[2]);
	BP_Option_ChaVolume->RefreshSizeText();

	BP_Option_EffectVolume->SetBarName(FString(TEXT("이펙트 볼륨")));
	BP_Option_EffectVolume->SetVolumeType(masterSClass->ChildClasses[2]->Properties.Volume);
	BP_Option_EffectVolume->SetPrevVolumeSize(allPrevSoundValue[3]);
	BP_Option_EffectVolume->SetMutable(allMuteStates[3]);
	BP_Option_EffectVolume->RefreshSizeText();

#pragma endregion


}

void UOptionWnd::SaveAllOptions()
{
#pragma region	SaveMuteState
	
	// 각 음소거값을 불러와서 배열에 저장합니다.
	TArray<bool> muteStates;
	muteStates.Add(BP_Option_MVolume->GetMutable());
	muteStates.Add(BP_Option_BackVolume->GetMutable());
	muteStates.Add(BP_Option_ChaVolume->GetMutable());
	muteStates.Add(BP_Option_EffectVolume->GetMutable());

#pragma endregion

#pragma region	SaveSoundValue

	// 각 사운드바에 적용되어있는 값을 불러와서 저장합니다.
	TArray<float> soundValue;
	soundValue.Add(BP_Option_MVolume->GetPrevVolumeSize());
	soundValue.Add(BP_Option_BackVolume->GetPrevVolumeSize());
	soundValue.Add(BP_Option_ChaVolume->GetPrevVolumeSize());
	soundValue.Add(BP_Option_EffectVolume->GetPrevVolumeSize());


#pragma endregion
	// 저장된 값으로 옵션을 저장합니다.
	GetHInstance()->SaveOptions(soundValue,muteStates);
}



void UOptionWnd::OnHomeButtonClicked()
{
	// 바꿨던 옵션을  모두 저장해줍니다.
	SaveAllOptions();
	// 캐릭터 데이터를 저장합니다.
	GetHInstance()->SaveCurrentData();

	// 저장이 완료된 뒤  홈으로 이동합니다.
	GetHInstance()->OpenNewMap(FName(TEXT("HomeLevel")));
}
void UOptionWnd::OnExitButtonClicked()
{
	//옵션을 저장합니다.
	SaveAllOptions();
	// 현재 캐릭터 데이터를 저장합니다.
	GetHInstance()->SaveCurrentData();

	// 게임을 종료합니다.
	UKismetSystemLibrary::QuitGame(GetWorld(),GetHInstance()->GetPlayerController(),EQuitPreference::Quit,true);
}

void UOptionWnd::OnOptionButtonClicked()
{
	// 옵션 버튼을 누르면 옵션설정창이 뜨게끔 합니다.
	WS_OptionSound->SetActiveWidgetIndex(0);
}

void UOptionWnd::OnSoundButtonClicked()
{
	// 사운드 버튼을 누르면 사운드 창이 뜨게끔합니다.
	WS_OptionSound->SetActiveWidgetIndex(1);
}
