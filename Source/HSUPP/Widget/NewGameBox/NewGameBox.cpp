// Fill out your copyright notice in the Description page of Project Settings.


#include "NewGameBox.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/CanvasPanelSlot.h"

#include "Widget/HomeWidget/HomeWidget.h"

void UNewGameBox::NativeConstruct()
{
	Super::NativeConstruct();
	
	//창의 기본 사이즈로 크기를 변경해줍니다.
	Cast<UCanvasPanelSlot>(this->Slot)->SetSize(FVector2D(800.0f,400.0f));

	//닉네임을 적는 칸에 텍스트가 들어올때의 처리할 행동을 바인딩하고 포커스를 텍스트박스로 맞춰줍니다.
	EditableText_NameText->OnTextChanged.AddDynamic(this, &UNewGameBox::OnTextChanged);
	EditableText_NameText->SetKeyboardFocus();

	// 취소 / 확인 버튼을 눌렀을때 불러올 메서드를 바인딩합니다.
	Button_Cancel->OnClicked.AddDynamic(this, &UNewGameBox::CloseThisWnd);
	Button_Ok->OnClicked.AddDynamic(this,&UNewGameBox::PressOkButton);
	

}

void UNewGameBox::OnTextChanged(const FText& Text)
{
	// 들어온 텍스트를 FString 형식으로 변경합니다.
	FString newText = Text.ToString();

	// 새로 들어온 텍스트가 띄어쓰기 일 경우를 위해 끝자리 여백을 지웁니다.
	newText = newText.TrimEnd();

	// 텍스트가 12자리가 넘게되면
	if (newText.Len() > 12)
	{
		// 12자리가 넘어가는 만큼 삭제합니다.
		int removeNum = newText.Len() - 12	;
		newText.RemoveAt(11, removeNum);
	}
	// 처리가 완료된 텍스트를 FText로 변환하여 박스에 적용해줍니다.
	FText newFText = FText::FromString(newText);
	EditableText_NameText->SetText(newFText);
	
}

void UNewGameBox::PressOkButton()
{
	// 확인을 누르면 텍스트 박스에 있던 텍스트를 FString으로 변환합니다.
	FString newCharacterName = EditableText_NameText->GetText().ToString();

	// 텍스트 박스가 빈칸일수도 있으니 검사합니다.
	if(newCharacterName == TEXT(""))
	{
		// 빈칸이라면 에러 메세지 박스를 설정하고 띄워줍니다.
		FText titleText;
		FText messageText;
		titleText = FText::FromString(TEXT("Error"));
		messageText = FText::FromString(TEXT("올바르지 않은 캐릭터 이름입니다."));
		GetHInstance()->GetHomeWidget()->CreateNoticeBox(titleText, messageText);
		return;
	}

	// 새로 캐릭터를 저장할 칸을 찾아냅니다.
	int newSlotNum = GetHInstance()->GetEmptySaveSlotNum();

	// 모든 칸이 사용중일경우
	if (newSlotNum == -1)
	{
		//칸이 꽉차서 더이상 만들수 없다는 메세지 창을 띄웁니다.
		FText titleText;
		FText messageText;
		titleText = FText::FromString(TEXT("Error"));
		messageText = FText::FromString(TEXT("더 이상 캐릭터를 생성할수 없습니다."));
		GetHInstance()->GetHomeWidget()->CreateNoticeBox(titleText, messageText);
	}
	else
	{
		// 캐릭터 이름을 FName 형식으로 저장하고 데이터를 저장하고 게임을 시작합니다.
		FName newCName = FName(*EditableText_NameText->GetText().ToString());
		GetHInstance()->CreateNewSaveData(newSlotNum, newCName);
		GetHInstance()->SetCurrentPlayerIndex(newSlotNum);
		GetHInstance()->OpenNewMap(FName(TEXT("VilageLevel")));
	}
}