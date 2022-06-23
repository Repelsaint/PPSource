// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"	
#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"

#include "Widget/InventoryWnd/InventoryWnd.h"
#include "Widget/Slot/EquipSlot/EquipItemSlot.h"
#include "Struct/ItemInfo.h"
#include "Widget/MainWidget.h"

UInventorySlot::UInventorySlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{	
	// 슬롯의 타입을 인벤토리 슬롯으로 지정합니다.
	SlotType = ESlotType::InventorySlot;
}



void UInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

void UInventorySlot::SlotChange()
{
	// 플레이어 캐릭터를 불러옵니다.
	APlayerCharacter* playerCharacter = GetHInstance()->GetPlayerCharacter();
	
	// 정보를 읽어오고 그 정보중 지금 클릭한 슬롯의 정보를 읽어옵니다.
	FPlayerInfo& playerInfo = playerCharacter->GetPlayerInfo();
	FItemInfo& thisItemInfo = playerInfo.GetInventoryInfo()[this->SlotIndex];


	// 빈칸에 우클릭을 하거나 포션이라면 실행을 취소합니다.
	if (thisItemInfo.ItemCode == 0) return;
	if (thisItemInfo.ItemType == EItemType::Potion) return;

	// 지금 클릭한 슬롯의 아이템과 같은 타입의 장비 칸을 찾습니다.
	FItemInfo& swapItemInfo = playerInfo.GetEquipInfo()[thisItemInfo.ItemType];

	// 메인위젯을 얻어옵니다.
	UMainWidget* mainWidget = GetHInstance()->GetMainWidget();

	
	// 이 타입과 맞는 타입의 슬롯이 비어있다면 장비칸에 아이템을 채우고 인벤토리칸을 비워줍니다.
	if (swapItemInfo.ItemCode == 0)
	{
		swapItemInfo = thisItemInfo;
		thisItemInfo = FItemInfo();
		
	}
	else
	{
		// 아이템 슬롯이 채워져있다면 아이템을  스왑해줍니다.
		FItemInfo swapData = playerInfo.GetEquipInfo()[thisItemInfo.ItemType];

		swapItemInfo = thisItemInfo;
		thisItemInfo = swapData;
	}


	// 인벤토리 슬롯의 이미지를 갱신합니다.
	GetHInstance()->GetMainWidget()->GetInventoryWnd()->GetInventorySlot(this->SlotIndex)->UpdateItemImage(thisItemInfo.ItemCode,thisItemInfo.ItemCount);
	
	// 아이템 스왑으로 인한 변경값을 얻어냅니다
	int changeValue = swapItemInfo.ItemValue - thisItemInfo.ItemValue;

	//장비창이 켜져있다면 인포텍스트를 바로 수정해주고 이미지를 갱신해줍니다.
	if (mainWidget->GetEquipWnd() != nullptr)
	{
		mainWidget->GetEquipWnd()->GetEquipSlot(swapItemInfo.ItemType)->UpdateItemImage(swapItemInfo.ItemCode, thisItemInfo.ItemCount);
		mainWidget->GetEquipWnd()->UpdateEquipInfo(swapItemInfo.ItemType, changeValue);
	}
	// 장비창이켜져있지 않다면 타입을 검사하여 캐릭터 정보를 직접변경합니다.
	else
	{
		//타입을 이 팔과 머리라면 공격력을 아니라면 체력을 올려줍니다.
		if (swapItemInfo.ItemType == EItemType::Arm || swapItemInfo.ItemType == EItemType::Head)
		{
			playerCharacter->AddPlayerAtk(changeValue);
		}
		else
		{
			// 최대체력을 변경해주고 메인위젯의 hp바의 수치를 조정해줍니다.
			playerCharacter->AddPlayerMaxHP(changeValue);
			mainWidget->SetHPBar(playerCharacter->GetPlayerCurrentHP(),playerCharacter->GetPlayerMaxHP());
		}
	}

}


bool UInventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	// 드래그를 시작한 슬롯을 얻어옵니다.
	UBaseSlot* OriginSlot = Cast<USlotDragOperation>(InOperation)->OriginSlot;

	// 드래그한 슬롯과 현재 슬롯이 같으면 실행하지 않습니다.
	if(OriginSlot == this) return false;

	if (OriginSlot->GetSlotType() == ESlotType::EquipSlot)
	{
		//슬롯의 타입이 장비창이라면 장비창과의 정보 교환을 합니다.
		SwapInvFromEquipSlot(Cast<UEquipItemSlot>(OriginSlot));
		return true;
	}
	else
	{
		// 슬롯 타입이 인벤토리라면 슬롯간의 정보를 교환합니다.
		SwapInvFromInventorySlot(Cast<UInventorySlot>(OriginSlot));
		return true;
	}

	return false;
}




void UInventorySlot::UpdateItemImage(int itemCode, int itemCount)
{
	Super::UpdateItemImage(itemCode, itemCount);

	// 아이템의 갯수가 0개 혹은 1개라면 텍스트를 비우고 2개이상이라면 텍스트에 표시를 해줍니다.
	if (itemCount <= 1)
	{
		Text_ItemCount->SetText(FText());
	}
	else
		Text_ItemCount->SetText(FText::FromString(FString::FromInt(itemCount)));
}


void UInventorySlot::SwapInvFromInventorySlot(UInventorySlot* swapInventorySlot)
{
	// 드래그를 시작한 인벤토리 슬롯의 인덱스를 얻어옵니다.
	int OriginIndex = swapInventorySlot->GetSlotIndex();

	// 인벤토리의 정보를 읽어옵니다.
	TMap<int, FItemInfo>& InventoryInfo = GetHInstance()->GetPlayerCharacter()->GetPlayerInfo().GetInventoryInfo();

	// 현재의 슬롯과 기존의 슬롯의 정보를 저장합니다.
	FItemInfo OriginItemInfo = InventoryInfo[OriginIndex];
	FItemInfo ThisItemInfo = InventoryInfo[this->SlotIndex];


	// 코드가 없으면  아이템 교환을 하지 않습니다.
	if (OriginItemInfo.ItemCode == 0) return;

	//아이템코드가 다르거나 소모품이 아니라면 단순히 위치를 바꿔줍니다.
	if (OriginItemInfo.ItemCode != ThisItemInfo.ItemCode || OriginItemInfo.ItemType != EItemType::Potion)
	{
		InventoryInfo[OriginIndex] = ThisItemInfo;
		InventoryInfo[this->SlotIndex] = OriginItemInfo;
	}
	// 같은 코드의 포션이라면 스왑이 아닌 아이템합체를 합니다.
	else if (OriginItemInfo.ItemType == EItemType::Potion)
	{
		// 드래그한 아이템의 수량을 대상슬롯에 더해줍니다.
		ThisItemInfo.ItemCount += OriginItemInfo.ItemCount;

		// 만약 두 아이템의 수의 합이 최대 수량 보다 많다면 최대수량까지만 이동시킵니다.
		if (ThisItemInfo.ItemCount >= ThisItemInfo.ItemMaxCount)
		{
			OriginItemInfo.ItemCount = (ThisItemInfo.ItemCount - ThisItemInfo.ItemMaxCount);
			ThisItemInfo.ItemCount = ThisItemInfo.ItemMaxCount;
		}
		else
		{
			// 최대 수량을 넘기지 않았다면 드래그한 슬롯의 코드와 아이템수량을 초기화해줍니다.
			OriginItemInfo.ItemCode = 0;
			OriginItemInfo.ItemCount = 0;
		}

		// 스왑이 완료된 정보를 실제 정보에 대입해줍니다.
		InventoryInfo[this->SlotIndex] = ThisItemInfo;
		InventoryInfo[OriginIndex] = OriginItemInfo;
	}

	// 아이템의 이미지를 갱신시켜줍니다.
	this->UpdateItemImage(InventoryInfo[this->SlotIndex].ItemCode, InventoryInfo[this->SlotIndex].ItemCount);
	swapInventorySlot->UpdateItemImage(InventoryInfo[OriginIndex].ItemCode, InventoryInfo[OriginIndex].ItemCount);

}


void UInventorySlot::SwapInvFromEquipSlot(UEquipItemSlot* swapEquipSlot)
{

	// 인벤토리와 장비칸의 정보를 읽어옵니다.
	TMap<EItemType, FItemInfo>& equipInfo = GetHInstance()->GetPlayerCharacter()->GetPlayerInfo().GetEquipInfo();
	TMap<int, FItemInfo>& InventoryInfo = GetHInstance()->GetPlayerCharacter()->GetPlayerInfo().GetInventoryInfo();

	// 각 슬롯에 저장된 정보를 읽어옵니다.
	FItemInfo OriginItemInfo = equipInfo[swapEquipSlot->GetSlotEquipType()];
	FItemInfo SwapItemInfo = InventoryInfo[this->SlotIndex];

	// 아이템의 타입이 같다면 아이템을 스왑해줍니다.
	if (OriginItemInfo.ItemType == SwapItemInfo.ItemType)
	{
		// 각 슬롯의 정보를 스왑합니다.
		equipInfo[swapEquipSlot->GetSlotEquipType()] = SwapItemInfo;
		InventoryInfo[this->SlotIndex] = OriginItemInfo;

		//스왑한 아이템의 차이를 계산합니다.
		SwapItemInfo.ItemValue -=  OriginItemInfo.ItemValue;

		//각 칸의 이미지를 갱신해줍니다.
		this->UpdateItemImage(InventoryInfo[this->SlotIndex].ItemCode, InventoryInfo[this->SlotIndex].ItemCount);
		swapEquipSlot->UpdateItemImage(equipInfo[swapEquipSlot->GetSlotEquipType()].ItemCode);

		//바뀐 정보를 인포텍스트와 캐릭터 정보에도 적용 해줍니다.
		GetHInstance()->GetMainWidget()->GetEquipWnd()->UpdateEquipInfo(SwapItemInfo.ItemType,SwapItemInfo.ItemValue);
		
	}
	else if (SwapItemInfo.ItemCode == 0)
	{
		// 빈칸으로 장비를 옮겼으므로 장비칸을 지우고 빈칸에 장비를 추가합니다.
		equipInfo[swapEquipSlot->GetSlotEquipType()] = FItemInfo();
		InventoryInfo[this->SlotIndex] = OriginItemInfo;

		//각 칸의 이미지를 갱신해줍니다.
		this->UpdateItemImage(InventoryInfo[this->SlotIndex].ItemCode, InventoryInfo[this->SlotIndex].ItemCount);
		swapEquipSlot->UpdateItemImage(equipInfo[swapEquipSlot->GetSlotEquipType()].ItemCode);

		// 장비창이 빈칸이 되었으므로 기존의 아이템 밸류만큼 값을 빼줍니다.
		GetHInstance()->GetMainWidget()->GetEquipWnd()->UpdateEquipInfo(OriginItemInfo.ItemType, -OriginItemInfo.ItemValue);
	}
	else
	{
		// 장비가 들어갈 빈슬롯의 인덱스를 얻어옵니다.
		int newSlotIndex = GetHInstance()->GetPlayerCharacter()->GetPlayerInfo().GetEmptyInventorySlot();
		
		
		//빈슬롯을 찾았다면 빈칸에 장비를 추가하고 장비창을 비웁니다.
		if (newSlotIndex != -1)
		{
			//장비슬롯을 비워주고 빈칸에 장비템을 추가합니다.
			equipInfo[swapEquipSlot->GetSlotEquipType()] = FItemInfo();
			InventoryInfo[newSlotIndex] = OriginItemInfo;

			//드래그한 장비칸과 새로 정보가 들어간 인벤토리칸의 이미지를 갱신합니다.
			UMainWidget* MainWidget = GetHInstance()->GetMainWidget();
			swapEquipSlot->UpdateItemImage(equipInfo[swapEquipSlot->GetSlotEquipType()].ItemCode, equipInfo[swapEquipSlot->GetSlotEquipType()].ItemCount);
			MainWidget->GetInventoryWnd()->GetInventorySlot(newSlotIndex)->UpdateItemImage(InventoryInfo[newSlotIndex].ItemCode, InventoryInfo[newSlotIndex].ItemCount);

			// 바뀐 정보를 토대로 인포텍스트의 값을 변경해줍니다.
			MainWidget->GetEquipWnd()->UpdateEquipInfo(OriginItemInfo.ItemType, -OriginItemInfo.ItemValue);
		}
		// 빈칸이 없다면 실행을 취소합니다.
		else
			return;
	}

}

void UInventorySlot::SetItemInfoFromCode(int itemCode)
{
	// 코드가 없다면 실행하지 않습니다.
	if(itemCode == 0) return;

	// 코드를 FString으로 변환하여 코드에 맞는 아이템 정보를 가져옵니다.
	FName itemCodeName = FName(*FString::FromInt(itemCode));
	FItemInfo* thisItemInfo = DT_ItemInfo->FindRow<FItemInfo>(itemCodeName,TEXT(""));

	// 슬롯의 정보를 저장합니다.
	GetHInstance()->GetPlayerCharacter()->GetPlayerInfo().InventoryInfo[SlotIndex].ItemType = thisItemInfo->ItemType;
	GetHInstance()->GetPlayerCharacter()->GetPlayerInfo().InventoryInfo[SlotIndex].ItemValue = thisItemInfo->ItemValue;
	GetHInstance()->GetPlayerCharacter()->GetPlayerInfo().InventoryInfo[SlotIndex].ItemMaxCount = thisItemInfo->ItemMaxCount;
}


void UInventorySlot::SetSlotIndex(int index)
{
	// 슬롯의 인덱스를 지정합니다.
	SlotIndex = index;
}

