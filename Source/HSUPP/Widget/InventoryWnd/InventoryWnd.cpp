// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWnd.h"

#include "components/Textblock.h"
#include "Components/GridPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/GridSlot.h"
#include "Widget/MainWidget.h"
#include "Struct/ItemInfo.h"

#include"Widget/Slot/InventorySlot/InventorySlot.h"

UInventoryWnd::UInventoryWnd(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// ������ �������Ʈ�� �ҷ��ɴϴ�.
	static ConstructorHelpers::FClassFinder<UInventorySlot> BP_INVENTORYSLOT(TEXT("WidgetBlueprint'/Game/Widget/PlayerInventory/InventorySlot/BP_InventorySlot.BP_InventorySlot_C'"));
	if (BP_INVENTORYSLOT.Succeeded())
		BP_InventorySlot = BP_INVENTORYSLOT.Class;


	//������ �⺻ ����� �����մϴ�.
	WndSize = FVector2D(414.0f,700.0f);


}

void UInventoryWnd::UpdateAllInventorySlot()
{
	// ��� �κ��丮�� ������ �о�ɴϴ�.
	TMap<int, FItemInfo>& InventoryInfo = GetHInstance()->GetPlayerCharacter()->GetPlayerInfo().InventoryInfo;

	// ��� ������ �̹����� �ڵ带 �������ݴϴ�.
	for (int i = 0; i < InventoryInfo.Num(); ++i)
	{
		SlotArray[i]->UpdateItemImage(InventoryInfo[i].ItemCode, InventoryInfo[i].ItemCount);
		SlotArray[i]->SetItemInfoFromCode(InventoryInfo[i].ItemCode);
	}
	// ����� �絵 ���������ݴϴ�.
	Text_Money->SetText(FText::FromString(FString::FromInt(GetHInstance()->GetPlayerCharacter()->GetPlayerInfo().Money)));
}


void UInventoryWnd::NativeConstruct()
{
	Super::NativeConstruct();

	// �÷��̾� ������ �ݾ����� �����ݾ��� �����մϴ�.
	Text_Money->SetText(FText::FromString(FString::FromInt(GetHInstance()->GetPlayerCharacter()->GetPlayerInfo().Money)));

	// â�� ������ �Է��ϰ� â�� �ʱ�ȭ�մϴ�.
	SetTitleText(FString(TEXT("Inventory")));
	IntializeInventoryWindow();

	
}

void UInventoryWnd::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	// �⺻ �������� ũ��� â�� ũ�⸦ �����մϴ�
	Cast<UCanvasPanelSlot>(Slot)->GetPosition();

}


void UInventoryWnd::IntializeInventoryWindow()
{
	// �����ٿ� 6���� ������ ����� ���ְ� �մϴ�.
	int WidthCount = 6;

	// �� ������ ������ �����Դϴ�.
	int SlotCount = 54;

	//�κ��丮�� �������� ǥ���ϱ����� ������ �ڵ带 �� �ҷ����Դϴ�.
	TMap<int, FItemInfo> &invInfo = GetHInstance()->GetPlayerCharacter()->GetPlayerInfo().GetInventoryInfo();
	
	//�迭�� �ʱ�ȭ���ݴϴ�.
	SlotArray.Empty();

	
	for (int i = 0; i < SlotCount; ++i)
	{
		// ���ο� ������ �����ϰ� ������Ʈ�� ����������Ʈ�� �߰��մϴ�.
		UInventorySlot* newSlot = CreateWidget<UInventorySlot>(this, BP_InventorySlot);
		GridPanel_SlotParent->AddChild(newSlot);

		// ������ �ε�����ȣ�� �����մϴ�.
		newSlot->SetSlotIndex(i);

		//������ �̹����� ������ ������ �����մϴ�.
		newSlot->UpdateItemImage(invInfo[i].ItemCode,invInfo[i].ItemCount);
		
		//������ ã���� ���� �迭�� ������ �־��ݴϴ�.
		SlotArray.Add(newSlot);

		// ������ ����/������ �� ���� ������ ���ݴϴ�.
		UGridSlot* newGridSlot = Cast<UGridSlot>(newSlot->Slot);
		
		newGridSlot->SetColumn(i % WidthCount);
		newGridSlot->SetRow(i / WidthCount);

		newGridSlot->SetPadding(FMargin(2.0f));
		
	}

}