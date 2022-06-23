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
	// ���� �ɼ� �������Ʈ�Դϴ�.
	UPROPERTY(meta = (BindWidget))
		USoundOptionBar* BP_Option_MVolume;
	UPROPERTY(meta = (BindWidget))
		USoundOptionBar* BP_Option_ChaVolume;
	UPROPERTY(meta = (BindWidget))
		USoundOptionBar* BP_Option_BackVolume;
	UPROPERTY(meta = (BindWidget))
		USoundOptionBar* BP_Option_EffectVolume;
		
	// ��ũ������ �ɼ� �������Ʈ�Դϴ�.
	UPROPERTY(meta = (BindWidget))
		UComboBoxString* ComboBox_WndMode;
	UPROPERTY(meta = (BindWidget))
		UComboBoxString* ComboBox_Resolution;
		 
	// �ɼ�â�� ������ �ٲٱ� ���ѹ�ư�Դϴ�.
	UPROPERTY(meta = (BindWidget))
		UButton* Button_Option;
	UPROPERTY(meta = (BindWidget))
		UButton* Button_Sound;

	// Ȩ / ���� ��ư�Դϴ�.
	UPROPERTY(meta = (BindWidget))
		UButton* Button_Exit;
	UPROPERTY(meta = (BindWidget))
		UButton* Button_Home;

	// â ������ ���� ����ġ�Դϴ�.
	UPROPERTY(meta = (BindWidget))
		UWidgetSwitcher* WS_OptionSound;


private:
	
	// ��ӹڽ��� �ɼ��� �ٲ������ ȣ��� �޼����Դϴ�.
	UFUNCTION()
	void OnResolutionSizeChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	UFUNCTION()
	void OnScreenSizeChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	// Ȩ / �����ư�� �������� ȣ��� �޼����Դϴ�.
	UFUNCTION()
	void OnHomeButtonClicked();
	UFUNCTION()
	void OnExitButtonClicked();

	// â ������ ���� ��ư�� �������� ȣ��� �޼����Դϴ�.
	UFUNCTION()
	void OnOptionButtonClicked();
	UFUNCTION()
	void OnSoundButtonClicked();

	
public:
	UOptionWnd(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	// �ɼ�â�� �ʱ�ȭ �մϴ�.
	void InitializeOptionWnd();

	// ��� �ɼ��� �����մϴ�.
	void SaveAllOptions();
};
