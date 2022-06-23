// Fill out your copyright notice in the Description page of Project Settings.


#include "NoticeBox.h"

#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"

void UNoticeBox::NativeConstruct()
{
	Super::NativeConstruct();

	// â�� �⺻ �������Դϴ�.
	WndSize = FVector2D(500.0f,300.0f);

	// �⺻������ â�� ������ ����� �ϰ� ȭ���� �߾��� ������ �ǰԲ��մϴ�.
	Cast<UCanvasPanelSlot>(this->Slot)->SetAnchors(FAnchors(0.5f, 0.5f));
	Cast<UCanvasPanelSlot>(this->Slot)->SetAlignment(FVector2D(0.5f, 0.5f));
	
	// �˸� ���ڰ� 3�ʵ� �ڵ����� ���� �ǰԲ� Ÿ�̸Ӹ� �����մϴ�.
	FTimerHandle newTimerHandle;
	GetHInstance()->GetTimerManager().SetTimer(newTimerHandle, this, &UNoticeBox::CloseThisWnd, 3.0f, false);
}

void UNoticeBox::SetMessageText(FText newMessageText)
{
	//�޼��� â�� ��� �޼����� ������ �����մϴ�.
	TextBlock_Message->SetText(newMessageText);
}
