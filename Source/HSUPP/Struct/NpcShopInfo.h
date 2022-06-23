// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Struct/ItemInfo.h"
#include "Engine/DataTable.h"

#include "NpcShopInfo.generated.h"

/**
 * 
 */
 USTRUCT(BlueprintType)
struct HSUPP_API FNpcShopInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ShopNpcCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int> ShopItemCodes;
};
