// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/DefaultWndBase.h"
#include "SelectCheckBox.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnMessageBoxChecked)


UCLASS()
class HSUPP_API USelectCheckBox : public UDefaultWndBase
{
	GENERATED_BODY()
	
private:
	// 메세지 박스에서 표시할 내용을 나타내는 컴포넌트입니다.
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextBlock_CheckMessage;

	// 확인 / 취소 버튼입니다.
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Ok;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Cancel;

public:
	virtual void NativeConstruct() override;

	// 확인 버튼을 눌렀을때 호출될 대리자입니다.
	FOnMessageBoxChecked OnMessageBoxChecked;

	// 메세지 텍스트를 원하는 내용으로 바꿉니다.
	void SetMessageText(FText messageText);
	
	// 확인 / 취소 버튼을 눌렀을 때 호출될 메서드입니다.
	UFUNCTION()
	void OnOKButtonClicked();
	UFUNCTION()
	void OnCancelButtonClicked();
};
