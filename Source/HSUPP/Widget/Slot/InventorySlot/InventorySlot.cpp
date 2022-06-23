// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"	
#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"

#include "Widget/InventoryWnd/InventoryWnd.h"
#include "Widget/Slot/EquipSlot/EquipItemSlot.h"
#include "Struct/ItemInfo.h"
#include "Widget/MainWidget.h"

UInventorySlot::UInventorySlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{	
	// ������ Ÿ���� �κ��丮 �������� �����մϴ�.
	SlotType = ESlotType::InventorySlot;
}



void UInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

void UInventorySlot::SlotChange()
{
	// �÷��̾� ĳ���͸� �ҷ��ɴϴ�.
	APlayerCharacter* playerCharacter = GetHInstance()->GetPlayerCharacter();
	
	// ������ �о���� �� ������ ���� Ŭ���� ������ ������ �о�ɴϴ�.
	FPlayerInfo& playerInfo = playerCharacter->GetPlayerInfo();
	FItemInfo& thisItemInfo = playerInfo.GetInventoryInfo()[this->SlotIndex];


	// ��ĭ�� ��Ŭ���� �ϰų� �����̶�� ������ ����մϴ�.
	if (thisItemInfo.ItemCode == 0) return;
	if (thisItemInfo.ItemType == EItemType::Potion) return;

	// ���� Ŭ���� ������ �����۰� ���� Ÿ���� ��� ĭ�� ã���ϴ�.
	FItemInfo& swapItemInfo = playerInfo.GetEquipInfo()[thisItemInfo.ItemType];

	// ���������� ���ɴϴ�.
	UMainWidget* mainWidget = GetHInstance()->GetMainWidget();

	
	// �� Ÿ�԰� �´� Ÿ���� ������ ����ִٸ� ���ĭ�� �������� ä��� �κ��丮ĭ�� ����ݴϴ�.
	if (swapItemInfo.ItemCode == 0)
	{
		swapItemInfo = thisItemInfo;
		thisItemInfo = FItemInfo();
		
	}
	else
	{
		// ������ ������ ä�����ִٸ� ��������  �������ݴϴ�.
		FItemInfo swapData = playerInfo.GetEquipInfo()[thisItemInfo.ItemType];

		swapItemInfo = thisItemInfo;
		thisItemInfo = swapData;
	}


	// �κ��丮 ������ �̹����� �����մϴ�.
	GetHInstance()->GetMainWidget()->GetInventoryWnd()->GetInventorySlot(this->SlotIndex)->UpdateItemImage(thisItemInfo.ItemCode,thisItemInfo.ItemCount);
	
	// ������ �������� ���� ���氪�� �����ϴ�
	int changeValue = swapItemInfo.ItemValue - thisItemInfo.ItemValue;

	//���â�� �����ִٸ� �����ؽ�Ʈ�� �ٷ� �������ְ� �̹����� �������ݴϴ�.
	if (mainWidget->GetEquipWnd() != nullptr)
	{
		mainWidget->GetEquipWnd()->GetEquipSlot(swapItemInfo.ItemType)->UpdateItemImage(swapItemInfo.ItemCode, thisItemInfo.ItemCount);
		mainWidget->GetEquipWnd()->UpdateEquipInfo(swapItemInfo.ItemType, changeValue);
	}
	// ���â���������� �ʴٸ� Ÿ���� �˻��Ͽ� ĳ���� ������ ���������մϴ�.
	else
	{
		//Ÿ���� �� �Ȱ� �Ӹ���� ���ݷ��� �ƴ϶�� ü���� �÷��ݴϴ�.
		if (swapItemInfo.ItemType == EItemType::Arm || swapItemInfo.ItemType == EItemType::Head)
		{
			playerCharacter->AddPlayerAtk(changeValue);
		}
		else
		{
			// �ִ�ü���� �������ְ� ���������� hp���� ��ġ�� �������ݴϴ�.
			playerCharacter->AddPlayerMaxHP(changeValue);
			mainWidget->SetHPBar(playerCharacter->GetPlayerCurrentHP(),playerCharacter->GetPlayerMaxHP());
		}
	}

}


bool UInventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	// �巡�׸� ������ ������ ���ɴϴ�.
	UBaseSlot* OriginSlot = Cast<USlotDragOperation>(InOperation)->OriginSlot;

	// �巡���� ���԰� ���� ������ ������ �������� �ʽ��ϴ�.
	if(OriginSlot == this) return false;

	if (OriginSlot->GetSlotType() == ESlotType::EquipSlot)
	{
		//������ Ÿ���� ���â�̶�� ���â���� ���� ��ȯ�� �մϴ�.
		SwapInvFromEquipSlot(Cast<UEquipItemSlot>(OriginSlot));
		return true;
	}
	else
	{
		// ���� Ÿ���� �κ��丮��� ���԰��� ������ ��ȯ�մϴ�.
		SwapInvFromInventorySlot(Cast<UInventorySlot>(OriginSlot));
		return true;
	}

	return false;
}




void UInventorySlot::UpdateItemImage(int itemCode, int itemCount)
{
	Super::UpdateItemImage(itemCode, itemCount);

	// �������� ������ 0�� Ȥ�� 1����� �ؽ�Ʈ�� ���� 2���̻��̶�� �ؽ�Ʈ�� ǥ�ø� ���ݴϴ�.
	if (itemCount <= 1)
	{
		Text_ItemCount->SetText(FText());
	}
	else
		Text_ItemCount->SetText(FText::FromString(FString::FromInt(itemCount)));
}


void UInventorySlot::SwapInvFromInventorySlot(UInventorySlot* swapInventorySlot)
{
	// �巡�׸� ������ �κ��丮 ������ �ε����� ���ɴϴ�.
	int OriginIndex = swapInventorySlot->GetSlotIndex();

	// �κ��丮�� ������ �о�ɴϴ�.
	TMap<int, FItemInfo>& InventoryInfo = GetHInstance()->GetPlayerCharacter()->GetPlayerInfo().GetInventoryInfo();

	// ������ ���԰� ������ ������ ������ �����մϴ�.
	FItemInfo OriginItemInfo = InventoryInfo[OriginIndex];
	FItemInfo ThisItemInfo = InventoryInfo[this->SlotIndex];


	// �ڵ尡 ������  ������ ��ȯ�� ���� �ʽ��ϴ�.
	if (OriginItemInfo.ItemCode == 0) return;

	//�������ڵ尡 �ٸ��ų� �Ҹ�ǰ�� �ƴ϶�� �ܼ��� ��ġ�� �ٲ��ݴϴ�.
	if (OriginItemInfo.ItemCode != ThisItemInfo.ItemCode || OriginItemInfo.ItemType != EItemType::Potion)
	{
		InventoryInfo[OriginIndex] = ThisItemInfo;
		InventoryInfo[this->SlotIndex] = OriginItemInfo;
	}
	// ���� �ڵ��� �����̶�� ������ �ƴ� ��������ü�� �մϴ�.
	else if (OriginItemInfo.ItemType == EItemType::Potion)
	{
		// �巡���� �������� ������ ��󽽷Կ� �����ݴϴ�.
		ThisItemInfo.ItemCount += OriginItemInfo.ItemCount;

		// ���� �� �������� ���� ���� �ִ� ���� ���� ���ٸ� �ִ���������� �̵���ŵ�ϴ�.
		if (ThisItemInfo.ItemCount >= ThisItemInfo.ItemMaxCount)
		{
			OriginItemInfo.ItemCount = (ThisItemInfo.ItemCount - ThisItemInfo.ItemMaxCount);
			ThisItemInfo.ItemCount = ThisItemInfo.ItemMaxCount;
		}
		else
		{
			// �ִ� ������ �ѱ��� �ʾҴٸ� �巡���� ������ �ڵ�� �����ۼ����� �ʱ�ȭ���ݴϴ�.
			OriginItemInfo.ItemCode = 0;
			OriginItemInfo.ItemCount = 0;
		}

		// ������ �Ϸ�� ������ ���� ������ �������ݴϴ�.
		InventoryInfo[this->SlotIndex] = ThisItemInfo;
		InventoryInfo[OriginIndex] = OriginItemInfo;
	}

	// �������� �̹����� ���Ž����ݴϴ�.
	this->UpdateItemImage(InventoryInfo[this->SlotIndex].ItemCode, InventoryInfo[this->SlotIndex].ItemCount);
	swapInventorySlot->UpdateItemImage(InventoryInfo[OriginIndex].ItemCode, InventoryInfo[OriginIndex].ItemCount);

}


void UInventorySlot::SwapInvFromEquipSlot(UEquipItemSlot* swapEquipSlot)
{

	// �κ��丮�� ���ĭ�� ������ �о�ɴϴ�.
	TMap<EItemType, FItemInfo>& equipInfo = GetHInstance()->GetPlayerCharacter()->GetPlayerInfo().GetEquipInfo();
	TMap<int, FItemInfo>& InventoryInfo = GetHInstance()->GetPlayerCharacter()->GetPlayerInfo().GetInventoryInfo();

	// �� ���Կ� ����� ������ �о�ɴϴ�.
	FItemInfo OriginItemInfo = equipInfo[swapEquipSlot->GetSlotEquipType()];
	FItemInfo SwapItemInfo = InventoryInfo[this->SlotIndex];

	// �������� Ÿ���� ���ٸ� �������� �������ݴϴ�.
	if (OriginItemInfo.ItemType == SwapItemInfo.ItemType)
	{
		// �� ������ ������ �����մϴ�.
		equipInfo[swapEquipSlot->GetSlotEquipType()] = SwapItemInfo;
		InventoryInfo[this->SlotIndex] = OriginItemInfo;

		//������ �������� ���̸� ����մϴ�.
		SwapItemInfo.ItemValue -=  OriginItemInfo.ItemValue;

		//�� ĭ�� �̹����� �������ݴϴ�.
		this->UpdateItemImage(InventoryInfo[this->SlotIndex].ItemCode, InventoryInfo[this->SlotIndex].ItemCount);
		swapEquipSlot->UpdateItemImage(equipInfo[swapEquipSlot->GetSlotEquipType()].ItemCode);

		//�ٲ� ������ �����ؽ�Ʈ�� ĳ���� �������� ���� ���ݴϴ�.
		GetHInstance()->GetMainWidget()->GetEquipWnd()->UpdateEquipInfo(SwapItemInfo.ItemType,SwapItemInfo.ItemValue);
		
	}
	else if (SwapItemInfo.ItemCode == 0)
	{
		// ��ĭ���� ��� �Ű����Ƿ� ���ĭ�� ����� ��ĭ�� ��� �߰��մϴ�.
		equipInfo[swapEquipSlot->GetSlotEquipType()] = FItemInfo();
		InventoryInfo[this->SlotIndex] = OriginItemInfo;

		//�� ĭ�� �̹����� �������ݴϴ�.
		this->UpdateItemImage(InventoryInfo[this->SlotIndex].ItemCode, InventoryInfo[this->SlotIndex].ItemCount);
		swapEquipSlot->UpdateItemImage(equipInfo[swapEquipSlot->GetSlotEquipType()].ItemCode);

		// ���â�� ��ĭ�� �Ǿ����Ƿ� ������ ������ �����ŭ ���� ���ݴϴ�.
		GetHInstance()->GetMainWidget()->GetEquipWnd()->UpdateEquipInfo(OriginItemInfo.ItemType, -OriginItemInfo.ItemValue);
	}
	else
	{
		// ��� �� �󽽷��� �ε����� ���ɴϴ�.
		int newSlotIndex = GetHInstance()->GetPlayerCharacter()->GetPlayerInfo().GetEmptyInventorySlot();
		
		
		//�󽽷��� ã�Ҵٸ� ��ĭ�� ��� �߰��ϰ� ���â�� ���ϴ�.
		if (newSlotIndex != -1)
		{
			//��񽽷��� ����ְ� ��ĭ�� ������� �߰��մϴ�.
			equipInfo[swapEquipSlot->GetSlotEquipType()] = FItemInfo();
			InventoryInfo[newSlotIndex] = OriginItemInfo;

			//�巡���� ���ĭ�� ���� ������ �� �κ��丮ĭ�� �̹����� �����մϴ�.
			UMainWidget* MainWidget = GetHInstance()->GetMainWidget();
			swapEquipSlot->UpdateItemImage(equipInfo[swapEquipSlot->GetSlotEquipType()].ItemCode, equipInfo[swapEquipSlot->GetSlotEquipType()].ItemCount);
			MainWidget->GetInventoryWnd()->GetInventorySlot(newSlotIndex)->UpdateItemImage(InventoryInfo[newSlotIndex].ItemCode, InventoryInfo[newSlotIndex].ItemCount);

			// �ٲ� ������ ���� �����ؽ�Ʈ�� ���� �������ݴϴ�.
			MainWidget->GetEquipWnd()->UpdateEquipInfo(OriginItemInfo.ItemType, -OriginItemInfo.ItemValue);
		}
		// ��ĭ�� ���ٸ� ������ ����մϴ�.
		else
			return;
	}

}

void UInventorySlot::SetItemInfoFromCode(int itemCode)
{
	// �ڵ尡 ���ٸ� �������� �ʽ��ϴ�.
	if(itemCode == 0) return;

	// �ڵ带 FString���� ��ȯ�Ͽ� �ڵ忡 �´� ������ ������ �����ɴϴ�.
	FName itemCodeName = FName(*FString::FromInt(itemCode));
	FItemInfo* thisItemInfo = DT_ItemInfo->FindRow<FItemInfo>(itemCodeName,TEXT(""));

	// ������ ������ �����մϴ�.
	GetHInstance()->GetPlayerCharacter()->GetPlayerInfo().InventoryInfo[SlotIndex].ItemType = thisItemInfo->ItemType;
	GetHInstance()->GetPlayerCharacter()->GetPlayerInfo().InventoryInfo[SlotIndex].ItemValue = thisItemInfo->ItemValue;
	GetHInstance()->GetPlayerCharacter()->GetPlayerInfo().InventoryInfo[SlotIndex].ItemMaxCount = thisItemInfo->ItemMaxCount;
}


void UInventorySlot::SetSlotIndex(int index)
{
	// ������ �ε����� �����մϴ�.
	SlotIndex = index;
}

