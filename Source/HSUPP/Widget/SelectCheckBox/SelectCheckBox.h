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
	// �޼��� �ڽ����� ǥ���� ������ ��Ÿ���� ������Ʈ�Դϴ�.
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextBlock_CheckMessage;

	// Ȯ�� / ��� ��ư�Դϴ�.
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Ok;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Cancel;

public:
	virtual void NativeConstruct() override;

	// Ȯ�� ��ư�� �������� ȣ��� �븮���Դϴ�.
	FOnMessageBoxChecked OnMessageBoxChecked;

	// �޼��� �ؽ�Ʈ�� ���ϴ� �������� �ٲߴϴ�.
	void SetMessageText(FText messageText);
	
	// Ȯ�� / ��� ��ư�� ������ �� ȣ��� �޼����Դϴ�.
	UFUNCTION()
	void OnOKButtonClicked();
	UFUNCTION()
	void OnCancelButtonClicked();
};
