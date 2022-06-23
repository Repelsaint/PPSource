// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSUPP.h"
#include "Widget/DefaultWndBase.h"
#include "Widget/Slot/InventorySlot/InventorySlot.h"

#include "InventoryWnd.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API UInventoryWnd : public UDefaultWndBase
{
	GENERATED_BODY()

private:
	//인벤토리 슬롯들이 생성될 위치입니다.
	UPROPERTY(meta = (BindWidget))
		class UGridPanel* GridPanel_SlotParent;

		// 소지금액을 표시할 위치입니다.
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Money;


	// 인벤토리 슬롯의 블루프린트입니다.
	TSubclassOf<class UInventorySlot> BP_InventorySlot;

	// 슬롯들이 저장될 배열입니다.
	TArray<UInventorySlot*> SlotArray;


private:
	//인벤토리를 초기화합니다.
	void IntializeInventoryWindow();

protected:

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UInventoryWnd(const FObjectInitializer& ObjectInitializer);	

	//인벤토리슬롯을 모두 초기화합니다.
	void UpdateAllInventorySlot();

	FORCEINLINE UInventorySlot* GetInventorySlot(int indexNum)
	{ return SlotArray[indexNum]; }
};
