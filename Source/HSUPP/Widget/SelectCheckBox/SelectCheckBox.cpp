// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectCheckBox.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void USelectCheckBox::SetMessageText(FText messageText)
{
	// 들어온 메세지로 내용을 변경해줍니다.
	TextBlock_CheckMessage->SetText(messageText);
}

void USelectCheckBox::NativeConstruct()
{
	Super::NativeConstruct();
	
	// 타이틀 제목을 지정합니다.
	SetTitleText(TEXT("확인"));
	
	// 체크박스의 크기를 지정합니다.
	Cast<UCanvasPanelSlot>(Slot)->SetSize(FVector2D(400.0f,200.0f));

	// 버튼을 눌렀을 때 호출될 메서드를 바인딩합니다.
	Button_Ok->OnClicked.AddDynamic(this, &USelectCheckBox::OnOKButtonClicked);
	Button_Cancel->OnClicked.AddDynamic(this, &USelectCheckBox::OnCancelButtonClicked);
}

void USelectCheckBox::OnOKButtonClicked()
{
	// 확인버튼을 누를시 실행되야할 메서드를 호출합니다.
	OnMessageBoxChecked.Broadcast();
	// 처리후에 창을 닫습니다.
	CloseThisWnd();
}

void USelectCheckBox::OnCancelButtonClicked()
{	
	//취소 버튼을 누르면 창을 닫습니다.
	CloseThisWnd();
}
