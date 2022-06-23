
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSUPP.h"

#include "Struct/ItemInfo.h"
#include "Enum/ItemType.h"


#include "PlayerInfo.generated.h"

/**
 * 
 */
USTRUCT()
struct HSUPP_API FPlayerInfo
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY()
		int Level;
	UPROPERTY()
		int Atk;
	UPROPERTY()
		int CurrentHP;
	UPROPERTY()
		int MaxHP;
	UPROPERTY()
		int Money;
	UPROPERTY()
		FName CharacterName;

	UPROPERTY()
		float Exp;

	UPROPERTY()
		TMap<int, FItemInfo> InventoryInfo;

	UPROPERTY()
		TMap<EItemType, FItemInfo> EquipInfo;


	FORCEINLINE TMap<EItemType, FItemInfo>& GetEquipInfo()
	{
		return EquipInfo;
	}
	FORCEINLINE TMap<int, FItemInfo> &GetInventoryInfo()
	{
		return InventoryInfo;
	}

	int GetEmptyInventorySlot();


	TArray<int> GetSameItemSlot(int itemCode);

	void ResetInfo();
};
