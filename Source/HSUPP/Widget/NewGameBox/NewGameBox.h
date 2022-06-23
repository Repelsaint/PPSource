// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/DefaultWndBase.h"
#include "NewGameBox.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API UNewGameBox : public UDefaultWndBase
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Ok;
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Cancel;
	UPROPERTY(meta = (BindWidget))
	class UEditableText* EditableText_NameText;



public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnTextChanged(const FText& Text);
	UFUNCTION()
	void PressOkButton();
	
};
