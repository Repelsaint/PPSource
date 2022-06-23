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
	// �������� ������Դϴ�.
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_ItemThumb;
	
	// �������� �̸��� �����Դϴ�.
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextBlock_ItemName;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextBlock_ItemCost;

	// �������� �����Ҷ� ���˴ϴ�.
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Buy;

	// �������� �����Դϴ�.
	class UDataTable* DT_ItemInfo;

	// ���� ������ ���� �������� �������Դϴ�.
	int ItemCode;
	int ItemCost;
	int MaxItemCount;

public:
	UShopSlot(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	// ������ ������ �ڵ带���Ͽ� �����մϴ�.
	void SetSlotInfo(int itemCode);

	// �������� ���Ÿ� �õ��մϴ�.
	UFUNCTION()
	void TryBuyItem();
	
};
