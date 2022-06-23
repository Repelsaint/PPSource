// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentWnd.h"
#include "Widget/Slot/InventorySlot/InventorySlot.h"
#include "Widget/MainWidget.h"
#include "Components/TextBlock.h"


UEquipmentWnd::UEquipmentWnd(const FObjectInitializer& objectinitializer) :Super(objectinitializer)
{
	//�⺻ â ����� �����մϴ�.
	WndSize = FVector2D(400.0f, 700.0f);
}

void UEquipmentWnd::InitializeEquipWnd()
{
	// �����ؽ�Ʈ�� ã�Ƽ� �����մϴ�.
	CharacterInfoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_CInfo")));
	// �����ؽ�Ʈ�� ����� ������ �ε��մϴ�.
	APlayerCharacter* playerCharacter = GetHInstance()->GetPlayerCharacter();
	FPlayerInfo& playerInfo = playerCharacter->GetPlayerInfo();

	
	// �ε��� ������ �����ؽ�Ʈ�� ����ݴϴ�
	FString charInfo = (TEXT("ATK = ") + FString::FromInt(playerCharacter->GetPlayerAtk()) + TEXT("\nHP = ") + FString::FromInt(playerCharacter->GetPlayerMaxHP()));
	CharacterInfoText->SetText(FText::FromString(charInfo));

	for (auto equipSlots : EquipSlots)
	{
		equipSlots.Value->UpdateItemImage(playerInfo.EquipInfo[equipSlots.Key].ItemCode);
	}
}

void UEquipmentWnd::NativeConstruct()
{
	Super::NativeConstruct();



	SetTitleText(TEXT("Equipment"));
	
	// ã�� ���԰�ü�̸����� �����մϴ�.
	TArray<FName> equipSlotName =
	{
		FName(TEXT("BP_EquipSlot_Head")),
		FName(TEXT("BP_EquipSlot_Top")),
		FName(TEXT("BP_EquipSlot_Arm")),
		FName(TEXT("BP_EquipSlot_Bottom")),
		FName(TEXT("BP_EquipSlot_Shoes"))
	};

	// �����̸����� ���԰�ü�� ã�� �迭�� �����մϴ�.
	for (FName slotName : equipSlotName)
	{
		UEquipItemSlot* newSlot = Cast<UEquipItemSlot>(GetWidgetFromName(slotName));

		EquipSlots.Add(newSlot->GetSlotEquipType(), newSlot);
	}


	// ���â�� �ʱ�ȭ�մϴ�.
	InitializeEquipWnd();

}

void UEquipmentWnd::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
}


void UEquipmentWnd::UpdateEquipInfo(EItemType itemType, int value)
{
	// �÷��̾� ĳ���͸� �ҷ��ɴϴ�.
	APlayerCharacter* playerCharacter= GetHInstance()->GetPlayerCharacter();

	if (itemType == EItemType::Arm || itemType == EItemType::Head)
	{
	// ������ ������ ���̳� �Ӹ���� ���ݷ��� ��½�ŵ�ϴ�.
		playerCharacter->AddPlayerAtk(value);
	}
	else
	{
	// �׷��� �ʴٸ� ü���� �÷��ݴϴ�.
		playerCharacter->AddPlayerMaxHP(value);
		MainWidget->SetHPBar(playerCharacter->GetPlayerCurrentHP(), playerCharacter->GetPlayerMaxHP());
	}

	// ����� ���� �����ؽ�Ʈ�� �������ݴϴ�.
	FString charInfo(TEXT("ATK = ") + FString::FromInt(playerCharacter->GetPlayerAtk()) + TEXT("\nHP = ") + FString::FromInt(playerCharacter->GetPlayerMaxHP()));
	CharacterInfoText->SetText(FText::FromString(charInfo));
}
