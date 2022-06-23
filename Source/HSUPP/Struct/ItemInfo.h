// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSUPP.h"
#include "Engine/DataTable.h"
#include "Enum/Itemtype.h"

#include "ItemInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct HSUPP_API FItemInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ItemCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ItemMaxCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ItemCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ItemValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath ItemIconPath;
};
