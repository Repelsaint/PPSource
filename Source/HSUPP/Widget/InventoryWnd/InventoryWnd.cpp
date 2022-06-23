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
	// 슬롯의 블루프린트를 불러옵니다.
	static ConstructorHelpers::FClassFinder<UInventorySlot> BP_INVENTORYSLOT(TEXT("WidgetBlueprint'/Game/Widget/PlayerInventory/InventorySlot/BP_InventorySlot.BP_InventorySlot_C'"));
	if (BP_INVENTORYSLOT.Succeeded())
		BP_InventorySlot = BP_INVENTORYSLOT.Class;


	//윈도우 기본 사이즈를 지정합니다.
	WndSize = FVector2D(414.0f,700.0f);


}

void UInventoryWnd::UpdateAllInventorySlot()
{
	// 모든 인벤토리의 정보를 읽어옵니다.
	TMap<int, FItemInfo>& InventoryInfo = GetHInstance()->GetPlayerCharacter()->GetPlayerInfo().InventoryInfo;

	// 모든 슬롯의 이미지와 코드를 리셋해줍니다.
	for (int i = 0; i < InventoryInfo.Num(); ++i)
	{
		SlotArray[i]->UpdateItemImage(InventoryInfo[i].ItemCode, InventoryInfo[i].ItemCount);
		SlotArray[i]->SetItemInfoFromCode(InventoryInfo[i].ItemCode);
	}
	// 골드의 양도 재지정해줍니다.
	Text_Money->SetText(FText::FromString(FString::FromInt(GetHInstance()->GetPlayerCharacter()->GetPlayerInfo().Money)));
}


void UInventoryWnd::NativeConstruct()
{
	Super::NativeConstruct();

	// 플레이어 정보의 금액으로 소지금액을 설정합니다.
	Text_Money->SetText(FText::FromString(FString::FromInt(GetHInstance()->GetPlayerCharacter()->GetPlayerInfo().Money)));

	// 창의 제목을 입력하고 창을 초기화합니다.
	SetTitleText(FString(TEXT("Inventory")));
	IntializeInventoryWindow();

	
}

void UInventoryWnd::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	// 기본 사이즈의 크기로 창의 크기를 지정합니다
	Cast<UCanvasPanelSlot>(Slot)->GetPosition();

}


void UInventoryWnd::IntializeInventoryWindow()
{
	// 가로줄에 6개의 아이템 목록이 떠있게 합니다.
	int WidthCount = 6;

	// 총 아이템 슬롯의 갯수입니다.
	int SlotCount = 54;

	//인벤토리의 아이템을 표기하기위해 아이템 코드를 싹 불러들입니다.
	TMap<int, FItemInfo> &invInfo = GetHInstance()->GetPlayerCharacter()->GetPlayerInfo().GetInventoryInfo();
	
	//배열을 초기화해줍니다.
	SlotArray.Empty();

	
	for (int i = 0; i < SlotCount; ++i)
	{
		// 새로운 슬롯을 생성하고 슬롯페어런트의 하위오브젝트로 추가합니다.
		UInventorySlot* newSlot = CreateWidget<UInventorySlot>(this, BP_InventorySlot);
		GridPanel_SlotParent->AddChild(newSlot);

		// 슬롯의 인덱스번호를 지정합니다.
		newSlot->SetSlotIndex(i);

		//슬롯의 이미지와 아이템 갯수를 지정합니다.
		newSlot->UpdateItemImage(invInfo[i].ItemCode,invInfo[i].ItemCount);
		
		//슬롯을 찾을때 사용될 배열에 슬롯을 넣어줍니다.
		SlotArray.Add(newSlot);

		// 슬롯의 가로/세로줄 과 간격 설정을 해줍니다.
		UGridSlot* newGridSlot = Cast<UGridSlot>(newSlot->Slot);
		
		newGridSlot->SetColumn(i % WidthCount);
		newGridSlot->SetRow(i / WidthCount);

		newGridSlot->SetPadding(FMargin(2.0f));
		
	}

}