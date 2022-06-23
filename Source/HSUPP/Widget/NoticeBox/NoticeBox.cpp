// Fill out your copyright notice in the Description page of Project Settings.


#include "NoticeBox.h"

#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"

void UNoticeBox::NativeConstruct()
{
	Super::NativeConstruct();

	// 창의 기본 사이즈입니다.
	WndSize = FVector2D(500.0f,300.0f);

	// 기본적으로 창의 원점을 가운데로 하고 화면의 중앙이 원점이 되게끔합니다.
	Cast<UCanvasPanelSlot>(this->Slot)->SetAnchors(FAnchors(0.5f, 0.5f));
	Cast<UCanvasPanelSlot>(this->Slot)->SetAlignment(FVector2D(0.5f, 0.5f));
	
	// 알림 상자가 3초뒤 자동으로 삭제 되게끔 타이머를 설정합니다.
	FTimerHandle newTimerHandle;
	GetHInstance()->GetTimerManager().SetTimer(newTimerHandle, this, &UNoticeBox::CloseThisWnd, 3.0f, false);
}

void UNoticeBox::SetMessageText(FText newMessageText)
{
	//메세지 창에 띄울 메세지로 내용을 변경합니다.
	TextBlock_Message->SetText(newMessageText);
}
