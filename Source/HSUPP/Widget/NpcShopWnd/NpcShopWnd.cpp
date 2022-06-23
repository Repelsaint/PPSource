// Fill out your copyright notice in the Description page of Project Settings.


#include "NpcShopWnd.h"

#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/CanvasPanelSlot.h"

#include "Widget/Slot/ShopSlot/ShopSlot.h"

#include "Engine/DataTable.h"

#include "Struct/NpcShopInfo.h"

UNpcShopWnd::UNpcShopWnd(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{	

	// ������ �Ǹ��� �������� ������ �ҷ��ɴϴ�.
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_SHOP_INFO(TEXT("DataTable'/Game/DataTable/DT_NpcShopInfo.DT_NpcShopInfo'"));
	if(DT_SHOP_INFO.Succeeded())
		DT_ShopItemInfo = DT_SHOP_INFO.Object;

	// ���� �Ǹ� ������ �ҷ��ɴϴ�.
	static ConstructorHelpers::FClassFinder<UShopSlot> BP_SHOP_SLOT(TEXT("WidgetBlueprint'/Game/Widget/NpcFunction/BP_ShopSlot.BP_ShopSlot_C'"));
	if(BP_SHOP_SLOT.Succeeded())
		BP_ShopSlot = BP_SHOP_SLOT.Class;

	// �⺻������ ����� �����մϴ�.
	WndSize = FVector2D(414.0f, 700.0f);

}

void UNpcShopWnd::InitShopItemInfo(int npcCode)
{
	// ������ �ڵ带 FName �������� ��ȯ�մϴ�.
	FName shopName = FName(*FString::FromInt(npcCode));

	// ������ �ڵ带 �̿��Ͽ� ������ ������ �о�ɴϴ�.
	FNpcShopInfo* newShopInfo = DT_ShopItemInfo->FindRow<FNpcShopInfo>(shopName,TEXT(""));
	
	// ������ �Ǹ� �����۵��� �ڵ带 �����մϴ�.
	NpcShopItemCodes = newShopInfo->ShopItemCodes;
	
	
	for (int i = 0; i < NpcShopItemCodes.Num(); ++i)
	{
		// ������ �Ǹ� ������ �����մϴ�.
		UShopSlot* newShopSlot = CreateWidget<UShopSlot>(this, BP_ShopSlot);

		// ���Կ� �Ǹ��� �������� ������ �Է��մϴ�.
		newShopSlot->SetSlotInfo(newShopInfo->ShopItemCodes[i]);

		// ������ ����â�� ��ũ�ѹڽ��� ���Դϴ�.
		ScrollBox_ShopBox->AddChild(newShopSlot);

		// ������ ũ��� �е��� �����մϴ�.
		Cast<UCanvasPanelSlot>(newShopSlot->Slot)->SetSize(FVector2D(370.0f,50.0f));
		Cast<UScrollBoxSlot>(newShopSlot->Slot)->SetPadding(2.0f);
	}
	
}

void UNpcShopWnd::InitShopWndPos()
{
	// �⺻ ��ġ�� â�� �������� �߾ӿ� ���� �´�� �Ǿ�� �ϹǷ� �������� �о �����մϴ�.
	Cast<UCanvasPanelSlot>(this->Slot)->SetPosition(FVector2D(-210.0f,0.0f));

	// â�� �̸��� �����մϴ�.
	SetTitleText(TEXT("Shop"));
}
