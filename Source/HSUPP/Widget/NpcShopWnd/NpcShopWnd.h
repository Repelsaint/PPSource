// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/DefaultWndBase.h"
#include "NpcShopWnd.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API UNpcShopWnd : public UDefaultWndBase
{
	GENERATED_BODY()
	
private:
	// 슬롯들을 붙일 부모객체입니다.
	UPROPERTY(meta = (BindWidget))
		class UScrollBox* ScrollBox_ShopBox;

	// 상점에서 판매할 아이템의 정보입니다.
	class UDataTable* DT_ShopItemInfo;
	
	// 슬롯 블루프린트입니다.
	TSubclassOf<class UShopSlot> BP_ShopSlot;

	// 판매할 아이템들의 코드입니다.
	TArray<int> NpcShopItemCodes;

public:
	UNpcShopWnd(const FObjectInitializer& ObjectInitializer);

	// 상점을 초기화할때 사용합니다.
	void InitShopItemInfo(int npcCode);

	// 상점 크기를 초기화 할때 사용합니다.
	void InitShopWndPos();
};
