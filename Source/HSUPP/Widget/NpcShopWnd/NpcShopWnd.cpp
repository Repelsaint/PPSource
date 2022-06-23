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

	// 상인이 판매할 아이템의 정보를 불러옵니다.
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_SHOP_INFO(TEXT("DataTable'/Game/DataTable/DT_NpcShopInfo.DT_NpcShopInfo'"));
	if(DT_SHOP_INFO.Succeeded())
		DT_ShopItemInfo = DT_SHOP_INFO.Object;

	// 상점 판매 슬롯을 불러옵니다.
	static ConstructorHelpers::FClassFinder<UShopSlot> BP_SHOP_SLOT(TEXT("WidgetBlueprint'/Game/Widget/NpcFunction/BP_ShopSlot.BP_ShopSlot_C'"));
	if(BP_SHOP_SLOT.Succeeded())
		BP_ShopSlot = BP_SHOP_SLOT.Class;

	// 기본윈도우 사이즈를 지정합니다.
	WndSize = FVector2D(414.0f, 700.0f);

}

void UNpcShopWnd::InitShopItemInfo(int npcCode)
{
	// 상인의 코드를 FName 형식으로 변환합니다.
	FName shopName = FName(*FString::FromInt(npcCode));

	// 상인의 코드를 이용하여 상점의 정보를 읽어옵니다.
	FNpcShopInfo* newShopInfo = DT_ShopItemInfo->FindRow<FNpcShopInfo>(shopName,TEXT(""));
	
	// 상점의 판매 아이템들의 코드를 저장합니다.
	NpcShopItemCodes = newShopInfo->ShopItemCodes;
	
	
	for (int i = 0; i < NpcShopItemCodes.Num(); ++i)
	{
		// 아이템 판매 슬롯을 생성합니다.
		UShopSlot* newShopSlot = CreateWidget<UShopSlot>(this, BP_ShopSlot);

		// 슬롯에 판매할 아이템의 정보를 입력합니다.
		newShopSlot->SetSlotInfo(newShopInfo->ShopItemCodes[i]);

		// 슬롯을 상점창의 스크롤박스에 붙입니다.
		ScrollBox_ShopBox->AddChild(newShopSlot);

		// 슬롯의 크기와 패딩을 지정합니다.
		Cast<UCanvasPanelSlot>(newShopSlot->Slot)->SetSize(FVector2D(370.0f,50.0f));
		Cast<UScrollBoxSlot>(newShopSlot->Slot)->SetPadding(2.0f);
	}
	
}

void UNpcShopWnd::InitShopWndPos()
{
	// 기본 위치가 창의 우측면이 중앙에 거의 맞닿게 되어야 하므로 왼쪽으로 밀어서 생성합니다.
	Cast<UCanvasPanelSlot>(this->Slot)->SetPosition(FVector2D(-210.0f,0.0f));

	// 창의 이름을 지정합니다.
	SetTitleText(TEXT("Shop"));
}
