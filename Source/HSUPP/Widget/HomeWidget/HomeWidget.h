// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HomeWidget.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API UHomeWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	// 홈위젯에 생성되어있는 버튼입니다.
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_NewGame;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_ContinueGame;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Setting;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Exit;
		// 창을 소환했을때 붙일 부모객체입니다.
	UPROPERTY(meta = (BindWidget))
		class UCanvasPanel* CanvasPanel_WndParent; 

		TSubclassOf<class UNewGameBox> BP_NewGameBox;
		class UNewGameBox* NewGameBox;

		TSubclassOf<class UNoticeBox> BP_NoticeBox;
		class UNoticeBox* NoticeBox;
		
		TSubclassOf<class USaveSlotWnd> BP_SaveSlotWnd;
		class USaveSlotWnd* SaveSlotWnd;

		TSubclassOf<class USelectCheckBox> BP_SelectCheckBox;
		class USelectCheckBox* SelectCheckBox;

		TSubclassOf<class UOptionWnd> BP_OptionWnd;
		class UOptionWnd* OptionWnd;

public:
	UHomeWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

	// 버튼을 눌렀을떄 호출될 메서드들입니다.
	UFUNCTION()
	void PressNewGameButton();
	UFUNCTION()
	void PressContinueButton();
	UFUNCTION()
	void PressSettingButton();
	UFUNCTION()
	void PressExitButton();

	// 표기해야할 메세지를 띄울때 사용되는 메서드입니다.
	void CreateNoticeBox(FText titleText,FText messageText);

	// 창을 닫습니다.
	void CloseWnd(class UDefaultWndBase* defaultWnd);

	// 체크 박스를 생성합니다.
	class USelectCheckBox* CreateCheckBox(FText messageText);

	FORCEINLINE USaveSlotWnd* GetSaveSlotWnd()
	{ return SaveSlotWnd; }
};
