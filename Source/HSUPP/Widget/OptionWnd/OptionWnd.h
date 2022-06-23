// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/DefaultWndBase.h"
#include "Widget/SoundOptionBar/SoundOptionBar.h"
#include "Components/ComboBoxString.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

#include "OptionWnd.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API UOptionWnd : public UDefaultWndBase
{
	GENERATED_BODY()

private:
	bool bIsClosing;

protected:
	// 사운드 옵션 블루프린트입니다.
	UPROPERTY(meta = (BindWidget))
		USoundOptionBar* BP_Option_MVolume;
	UPROPERTY(meta = (BindWidget))
		USoundOptionBar* BP_Option_ChaVolume;
	UPROPERTY(meta = (BindWidget))
		USoundOptionBar* BP_Option_BackVolume;
	UPROPERTY(meta = (BindWidget))
		USoundOptionBar* BP_Option_EffectVolume;
		
	// 스크린관련 옵션 블루프린트입니다.
	UPROPERTY(meta = (BindWidget))
		UComboBoxString* ComboBox_WndMode;
	UPROPERTY(meta = (BindWidget))
		UComboBoxString* ComboBox_Resolution;
		 
	// 옵션창의 종류를 바꾸기 위한버튼입니다.
	UPROPERTY(meta = (BindWidget))
		UButton* Button_Option;
	UPROPERTY(meta = (BindWidget))
		UButton* Button_Sound;

	// 홈 / 종료 버튼입니다.
	UPROPERTY(meta = (BindWidget))
		UButton* Button_Exit;
	UPROPERTY(meta = (BindWidget))
		UButton* Button_Home;

	// 창 변경을 위한 스윗치입니다.
	UPROPERTY(meta = (BindWidget))
		UWidgetSwitcher* WS_OptionSound;


private:
	
	// 드롭박스로 옵션이 바뀌었을때 호출될 메서드입니다.
	UFUNCTION()
	void OnResolutionSizeChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	UFUNCTION()
	void OnScreenSizeChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	// 홈 / 종료버튼을 눌렀을때 호출될 메서드입니다.
	UFUNCTION()
	void OnHomeButtonClicked();
	UFUNCTION()
	void OnExitButtonClicked();

	// 창 변경을 위해 버튼이 눌렸을때 호출될 메서드입니다.
	UFUNCTION()
	void OnOptionButtonClicked();
	UFUNCTION()
	void OnSoundButtonClicked();

	
public:
	UOptionWnd(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	// 옵션창을 초기화 합니다.
	void InitializeOptionWnd();

	// 모든 옵션을 저장합니다.
	void SaveAllOptions();
};
