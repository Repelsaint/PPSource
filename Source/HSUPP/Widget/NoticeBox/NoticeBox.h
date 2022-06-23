// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/DefaultWndBase.h"
#include "NoticeBox.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API UNoticeBox : public UDefaultWndBase
{
	GENERATED_BODY()
private:
	//화면에 띄워질 메세지텍스트입니다.
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextBlock_Message;

public:
	virtual void NativeConstruct() override;

	void SetMessageText(FText newMessageText);

};
