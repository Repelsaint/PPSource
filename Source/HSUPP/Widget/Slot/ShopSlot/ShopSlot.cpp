// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopSlot.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"

#include "Widget/TradeCheckBox/TradeCheckBox.h"
#include "Widget/MainWidget.h"

#include "Engine/Texture2D.h"

#include "Struct/ItemInfo.h"

UShopSlot::UShopSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// 아이템의 정보입니다.
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ITEMINFO(TEXT("DataTable'/Game/DataTable/DT_ItemInfo.DT_ItemInfo'"));
	if(DT_ITEMINFO.Succeeded())
		DT_ItemInfo = DT_ITEMINFO.Object;

}

void UShopSlot::NativeConstruct()
{
	// 구매 버튼을 누를때 호출될 메서드를 바인딩합니다.
	Button_Buy->OnClicked.AddDynamic(this, &UShopSlot::TryBuyItem);
}

void UShopSlot::SetSlotInfo(int itemCode)
{
	//코드를 FName형식으로 변환합니다.
	FName itemCodeName = FName(*FString::FromInt(itemCode));

	// FName 형식의 코드로 필요한 데이터 정보를 읽어옵니다.
	FItemInfo* thisItemInfo = DT_ItemInfo->FindRow<FItemInfo>(itemCodeName, TEXT(""));
	UTexture2D* newIconTexture = Cast<UTexture2D>(GetStreamManager()->LoadSynchronous(thisItemInfo->ItemIconPath));

	//불러온 정보로 이미지와 이름 가격을 설정합니다.
	Image_ItemThumb->SetBrushFromTexture(newIconTexture);
	TextBlock_ItemName->SetText(thisItemInfo->ItemName);
	TextBlock_ItemCost->SetText(FText::FromString(FString::FromInt(thisItemInfo->ItemCost)));

	// 아이템의 정보를 슬롯에 저장합니다.
	ItemCode = itemCode;
	ItemCost = thisItemInfo->ItemCost;
	MaxItemCount = thisItemInfo->ItemMaxCount;

}

void UShopSlot::TryBuyItem()
{
	// 이미 거래중이라면 실행시키지 않습니다.
	if(GetHInstance()->GetMainWidget()->GetIsTrade()) return;

	// 유저의 잔돈을 읽어옵니다.
	int& money = GetHInstance()->GetPlayerCharacter()->GetPlayerInfo().Money;

	// 유저가 가진 돈이 물건의 가격보다 높을경우에만 실행합니다. 
	if (ItemCost < money)
	{
		GetHInstance()->GetMainWidget()->CreateTradeCheckBox(ItemCode,MaxItemCount,ItemCost,money);
	}

}