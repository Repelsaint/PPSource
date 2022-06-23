// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSUPP.h"
#include "Widget/Slot/BaseSlot.h"
#include "Components/TextBlock.h"

#include "InventorySlot.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API UInventorySlot final : public UBaseSlot
{
	GENERATED_BODY()

private:

	// 인벤토리에 표기될 아이템의 갯수
	UPROPERTY(meta = (BindWidget))
		UTextBlock*	Text_ItemCount;

	// 슬롯의 인덱스 번호
	int SlotIndex;


protected:

	// 해당 위젯에 드래그 이벤트가 발생했을 경우 호출됩니다.
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	// 우클릭을 누르면 장비와 인벤토리의 슬롯을 변경합니다.
	virtual void SlotChange() override;

	// 드래그가 끝났을 경우 호출되는 메서드
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;


public:
	UInventorySlot(const FObjectInitializer& ObjectInitializer);

	// 슬롯의 아이템 이미지를 업데이트합니다.
	virtual void UpdateItemImage(int itemCode, int itemCount = 1) override;

	// 인벤토리 슬롯끼리의 정보를 교환합니다.
	void SwapInvFromInventorySlot(UInventorySlot* swapInventorySlot);

	// 장비슬롯과 인벤토리 슬롯끼리의 정보를 교환합니다.
	void SwapInvFromEquipSlot(class UEquipItemSlot* swapEquipSlot);

	// 코드로 슬롯의 아이템 정보를 저장합니다.
	void SetItemInfoFromCode(int itemCode);	

	// 슬롯 인덱스를 저장합니다.
	void SetSlotIndex(int index);

	FORCEINLINE int GetSlotIndex() const
	{
		return SlotIndex;
	}

};
