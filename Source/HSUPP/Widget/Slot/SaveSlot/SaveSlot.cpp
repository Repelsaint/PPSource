// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveSlot.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"

#include "Single/HSaveSystem/HSaveSystem.h"

#include "Widget/HomeWidget/HomeWidget.h"
#include "WIdget/SaveSlotWnd/SaveSlotWnd.h"

#include "Struct/PlayerInfo.h"


USaveSlot::USaveSlot(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	// 슬롯에 데이터가 있을경우 빈슬롯 이미지를 지우기 위한 공백이미지
	static ConstructorHelpers::FObjectFinder<UTexture2D> NONE_IMAGE(TEXT("Texture2D'/Game/Resources/Slot/T_NULL.T_NULL'"));
	if(NONE_IMAGE.Succeeded())
		NullImage = NONE_IMAGE.Object;

	 //슬롯에 데이터가 없을경우 사용할 이미지
	static ConstructorHelpers::FObjectFinder<UTexture2D> EMPTY_SLOT_IMAGE(TEXT("Texture2D'/Game/Resources/Image/EmptySlotImage.EmptySlotImage'"));
	if(EMPTY_SLOT_IMAGE.Succeeded())
		EmptySlotImage = EMPTY_SLOT_IMAGE.Object;

	// 비어있는지를 검사하는 변수
	bEmpty= true;
		

}

void USaveSlot::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼을 눌렀을때 호출할 메서드를 바인딩합니다.
	Button_Select->OnClicked.AddDynamic(this, &USaveSlot::SelectSlot);

	// 선택 / 취소 시에 색을 바꿀때 사용될 칼라입니다.
	NormalColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
	PressedColor = FLinearColor(0.451763f,0.532443f,1.0f,0.7f);
}

void USaveSlot::InitializeSaveSlot()
{
	// 플레이어 정보를 참조합니다.
	FPlayerInfo& playerInfo = GetHInstance()->LoadData(SlotIndex)->PlayerInfo;

	// 일반색으로 적용합니다.
	SetNormalColor();

	// 정보가존재하지 않는다면
	if (playerInfo.Level == 0)
	{
		// 이름을 비웁니다.
		TextBlock_CharacterName->SetText(FText());
		// 레벨 텍스트를 지웁니다.
		TextBlock_Level->SetText(FText());
		// 백그라운드 이미지를 EMptyImage로 지정합니다.
		Image_EmptySlot->SetBrushFromTexture(EmptySlotImage);

		// 비어있음을 체크합니다.
		bEmpty = true;

	}
	else
	{
		// 캐릭터 이름을 FString 형식으로 변환해서 이름을 변경합니다.
		FText characterName = FText::FromString(playerInfo.CharacterName.ToString());
		TextBlock_CharacterName->SetText(characterName);

		// 현재 경험치를 퍼센트로 계산합니다.
		float currentPercent = ((playerInfo.Exp * 100) / (playerInfo.Level * 10));

		// 현재 레벨과 경험치퍼센트를 FText 형식으로 변환합니다.
		FText characterLevel = FText::Format(FText::FromString(TEXT("Lv.{0} ({1}%)")), playerInfo.Level, currentPercent);

		// 텍스트를 지정하고 빈칸 이미지를 비워줍니다.
		TextBlock_Level->SetText(characterLevel);
		Image_EmptySlot->SetBrushFromTexture(NullImage);
		// 비어있지 않음을 체크합니다.
		bEmpty = false;
	}


}

void USaveSlot::SelectSlot()
{
	// 비어있으면 선택할수 없도록 합니다.
	if(bEmpty) return;

	// 선택했음을 색으로 표기해줍니다.
	SetColorAndOpacity(PressedColor);

	// 슬롯 인덱스를 지정합니다.
	GetHInstance()->GetHomeWidget()->GetSaveSlotWnd()->ChangeSelectedSlot(SlotIndex);
}

void USaveSlot::SetNormalColor()
{
	// 슬롯의 색을 원래대로 돌립니다.
	SetColorAndOpacity(NormalColor);
}
