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
	//ȭ�鿡 ����� �޼����ؽ�Ʈ�Դϴ�.
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextBlock_Message;

public:
	virtual void NativeConstruct() override;

	void SetMessageText(FText newMessageText);

};
