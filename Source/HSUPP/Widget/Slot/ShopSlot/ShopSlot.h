// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSUPP.h"
#include "Blueprint/UserWidget.h"
#include "ShopSlot.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API UShopSlot : public UUserWidget
{
	GENERATED_BODY()

private:
	// 아이템의 썸네일입니다.
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_ItemThumb;
	
	// 아이템의 이름과 가격입니다.
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextBlock_ItemName;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextBlock_ItemCost;

	// 아이템을 구매할때 사용됩니다.
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Buy;

	// 아이템의 정보입니다.
	class UDataTable* DT_ItemInfo;

	// 현재 슬롯이 지닌 아이템의 정보들입니다.
	int ItemCode;
	int ItemCost;
	int MaxItemCount;

public:
	UShopSlot(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	// 슬롯의 정보를 코드를통하여 저장합니다.
	void SetSlotInfo(int itemCode);

	// 아이템의 구매를 시도합니다.
	UFUNCTION()
	void TryBuyItem();
	
};
