// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfo.h"

int FPlayerInfo::GetEmptyInventorySlot()
{
	for (int i = 0; i < InventoryInfo.Num(); ++i)
	{
		if (InventoryInfo[i].ItemCode == 0)
		{
			return i;
		}
	}

	return -1;
}

TArray<int> FPlayerInfo::GetSameItemSlot(int itemCode)
{
	TArray<int> itemSlots;

	for (int i = 0; i < InventoryInfo.Num(); ++i)
	{
		if(InventoryInfo[i].ItemCode == itemCode)
			itemSlots.Add(i);
	}
	
	return itemSlots;
}

void FPlayerInfo::ResetInfo()
{
	Level = 1;
	Atk = 10;
	MaxHP = CurrentHP = 300;
	Money = 0;
	Exp = 0.0f;
	CharacterName = FName();
	InventoryInfo.Reset();

	for (int i = 0; i < 54; ++i)
	{
		InventoryInfo.Add(i, FItemInfo());
	}	

	EquipInfo.Reset();
	

	EquipInfo.Add(EItemType::Head, FItemInfo());
	EquipInfo.Add(EItemType::Top, FItemInfo());
	EquipInfo.Add(EItemType::Bottom, FItemInfo());
	EquipInfo.Add(EItemType::Arm, FItemInfo());
	EquipInfo.Add(EItemType::Shoes, FItemInfo());


	//임시 아이템 부여 테스트 코드
	InventoryInfo[30].ItemCode = 11001;
	InventoryInfo[30].ItemType = EItemType::Arm;
	InventoryInfo[30].ItemValue = 100;

	InventoryInfo[50].ItemCode = 13001;
	InventoryInfo[50].ItemType = EItemType::Arm;
	InventoryInfo[50].ItemValue = 300;


	InventoryInfo[52].ItemCode = 13000;
	InventoryInfo[52].ItemType = EItemType::Shoes;
	InventoryInfo[52].ItemValue = 200;

	InventoryInfo[12].ItemCode = 12000;
	InventoryInfo[12].ItemType = EItemType::Shoes;
	InventoryInfo[12].ItemValue = 400;

	InventoryInfo[51].ItemCode = 16000;
	InventoryInfo[51].ItemCount = 1;
	InventoryInfo[51].ItemType= EItemType::Potion;
	InventoryInfo[51].ItemMaxCount = 99;

	InventoryInfo[20].ItemCode = 16000;
	InventoryInfo[20].ItemCount = 90;
	InventoryInfo[20].ItemType = EItemType::Potion;
	InventoryInfo[20].ItemMaxCount = 99;


	InventoryInfo[25].ItemCode = 16000;
	InventoryInfo[25].ItemCount = 20;
	InventoryInfo[25].ItemType = EItemType::Potion;
	InventoryInfo[25].ItemMaxCount = 99;

}
