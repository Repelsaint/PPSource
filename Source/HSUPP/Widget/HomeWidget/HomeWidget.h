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
	// Ȩ������ �����Ǿ��ִ� ��ư�Դϴ�.
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_NewGame;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_ContinueGame;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Setting;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Exit;
		// â�� ��ȯ������ ���� �θ�ü�Դϴ�.
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

	// ��ư�� �������� ȣ��� �޼�����Դϴ�.
	UFUNCTION()
	void PressNewGameButton();
	UFUNCTION()
	void PressContinueButton();
	UFUNCTION()
	void PressSettingButton();
	UFUNCTION()
	void PressExitButton();

	// ǥ���ؾ��� �޼����� ��ﶧ ���Ǵ� �޼����Դϴ�.
	void CreateNoticeBox(FText titleText,FText messageText);

	// â�� �ݽ��ϴ�.
	void CloseWnd(class UDefaultWndBase* defaultWnd);

	// üũ �ڽ��� �����մϴ�.
	class USelectCheckBox* CreateCheckBox(FText messageText);

	FORCEINLINE USaveSlotWnd* GetSaveSlotWnd()
	{ return SaveSlotWnd; }
};
