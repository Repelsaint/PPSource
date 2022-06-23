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
	// ���Ե��� ���� �θ�ü�Դϴ�.
	UPROPERTY(meta = (BindWidget))
		class UScrollBox* ScrollBox_ShopBox;

	// �������� �Ǹ��� �������� �����Դϴ�.
	class UDataTable* DT_ShopItemInfo;
	
	// ���� �������Ʈ�Դϴ�.
	TSubclassOf<class UShopSlot> BP_ShopSlot;

	// �Ǹ��� �����۵��� �ڵ��Դϴ�.
	TArray<int> NpcShopItemCodes;

public:
	UNpcShopWnd(const FObjectInitializer& ObjectInitializer);

	// ������ �ʱ�ȭ�Ҷ� ����մϴ�.
	void InitShopItemInfo(int npcCode);

	// ���� ũ�⸦ �ʱ�ȭ �Ҷ� ����մϴ�.
	void InitShopWndPos();
};
