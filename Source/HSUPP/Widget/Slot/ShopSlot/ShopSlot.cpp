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
	// �������� �����Դϴ�.
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ITEMINFO(TEXT("DataTable'/Game/DataTable/DT_ItemInfo.DT_ItemInfo'"));
	if(DT_ITEMINFO.Succeeded())
		DT_ItemInfo = DT_ITEMINFO.Object;

}

void UShopSlot::NativeConstruct()
{
	// ���� ��ư�� ������ ȣ��� �޼��带 ���ε��մϴ�.
	Button_Buy->OnClicked.AddDynamic(this, &UShopSlot::TryBuyItem);
}

void UShopSlot::SetSlotInfo(int itemCode)
{
	//�ڵ带 FName�������� ��ȯ�մϴ�.
	FName itemCodeName = FName(*FString::FromInt(itemCode));

	// FName ������ �ڵ�� �ʿ��� ������ ������ �о�ɴϴ�.
	FItemInfo* thisItemInfo = DT_ItemInfo->FindRow<FItemInfo>(itemCodeName, TEXT(""));
	UTexture2D* newIconTexture = Cast<UTexture2D>(GetStreamManager()->LoadSynchronous(thisItemInfo->ItemIconPath));

	//�ҷ��� ������ �̹����� �̸� ������ �����մϴ�.
	Image_ItemThumb->SetBrushFromTexture(newIconTexture);
	TextBlock_ItemName->SetText(thisItemInfo->ItemName);
	TextBlock_ItemCost->SetText(FText::FromString(FString::FromInt(thisItemInfo->ItemCost)));

	// �������� ������ ���Կ� �����մϴ�.
	ItemCode = itemCode;
	ItemCost = thisItemInfo->ItemCost;
	MaxItemCount = thisItemInfo->ItemMaxCount;

}

void UShopSlot::TryBuyItem()
{
	// �̹� �ŷ����̶�� �����Ű�� �ʽ��ϴ�.
	if(GetHInstance()->GetMainWidget()->GetIsTrade()) return;

	// ������ �ܵ��� �о�ɴϴ�.
	int& money = GetHInstance()->GetPlayerCharacter()->GetPlayerInfo().Money;

	// ������ ���� ���� ������ ���ݺ��� ������쿡�� �����մϴ�. 
	if (ItemCost < money)
	{
		GetHInstance()->GetMainWidget()->CreateTradeCheckBox(ItemCode,MaxItemCount,ItemCost,money);
	}

}