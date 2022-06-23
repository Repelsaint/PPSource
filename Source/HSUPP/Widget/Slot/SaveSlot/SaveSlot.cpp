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
	// ���Կ� �����Ͱ� ������� �󽽷� �̹����� ����� ���� �����̹���
	static ConstructorHelpers::FObjectFinder<UTexture2D> NONE_IMAGE(TEXT("Texture2D'/Game/Resources/Slot/T_NULL.T_NULL'"));
	if(NONE_IMAGE.Succeeded())
		NullImage = NONE_IMAGE.Object;

	 //���Կ� �����Ͱ� ������� ����� �̹���
	static ConstructorHelpers::FObjectFinder<UTexture2D> EMPTY_SLOT_IMAGE(TEXT("Texture2D'/Game/Resources/Image/EmptySlotImage.EmptySlotImage'"));
	if(EMPTY_SLOT_IMAGE.Succeeded())
		EmptySlotImage = EMPTY_SLOT_IMAGE.Object;

	// ����ִ����� �˻��ϴ� ����
	bEmpty= true;
		

}

void USaveSlot::NativeConstruct()
{
	Super::NativeConstruct();

	// ��ư�� �������� ȣ���� �޼��带 ���ε��մϴ�.
	Button_Select->OnClicked.AddDynamic(this, &USaveSlot::SelectSlot);

	// ���� / ��� �ÿ� ���� �ٲܶ� ���� Į���Դϴ�.
	NormalColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
	PressedColor = FLinearColor(0.451763f,0.532443f,1.0f,0.7f);
}

void USaveSlot::InitializeSaveSlot()
{
	// �÷��̾� ������ �����մϴ�.
	FPlayerInfo& playerInfo = GetHInstance()->LoadData(SlotIndex)->PlayerInfo;

	// �Ϲݻ����� �����մϴ�.
	SetNormalColor();

	// �������������� �ʴ´ٸ�
	if (playerInfo.Level == 0)
	{
		// �̸��� ���ϴ�.
		TextBlock_CharacterName->SetText(FText());
		// ���� �ؽ�Ʈ�� ����ϴ�.
		TextBlock_Level->SetText(FText());
		// ��׶��� �̹����� EMptyImage�� �����մϴ�.
		Image_EmptySlot->SetBrushFromTexture(EmptySlotImage);

		// ��������� üũ�մϴ�.
		bEmpty = true;

	}
	else
	{
		// ĳ���� �̸��� FString �������� ��ȯ�ؼ� �̸��� �����մϴ�.
		FText characterName = FText::FromString(playerInfo.CharacterName.ToString());
		TextBlock_CharacterName->SetText(characterName);

		// ���� ����ġ�� �ۼ�Ʈ�� ����մϴ�.
		float currentPercent = ((playerInfo.Exp * 100) / (playerInfo.Level * 10));

		// ���� ������ ����ġ�ۼ�Ʈ�� FText �������� ��ȯ�մϴ�.
		FText characterLevel = FText::Format(FText::FromString(TEXT("Lv.{0} ({1}%)")), playerInfo.Level, currentPercent);

		// �ؽ�Ʈ�� �����ϰ� ��ĭ �̹����� ����ݴϴ�.
		TextBlock_Level->SetText(characterLevel);
		Image_EmptySlot->SetBrushFromTexture(NullImage);
		// ������� ������ üũ�մϴ�.
		bEmpty = false;
	}


}

void USaveSlot::SelectSlot()
{
	// ��������� �����Ҽ� ������ �մϴ�.
	if(bEmpty) return;

	// ���������� ������ ǥ�����ݴϴ�.
	SetColorAndOpacity(PressedColor);

	// ���� �ε����� �����մϴ�.
	GetHInstance()->GetHomeWidget()->GetSaveSlotWnd()->ChangeSelectedSlot(SlotIndex);
}

void USaveSlot::SetNormalColor()
{
	// ������ ���� ������� �����ϴ�.
	SetColorAndOpacity(NormalColor);
}
