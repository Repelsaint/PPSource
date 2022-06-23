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

	// �κ��丮�� ǥ��� �������� ����
	UPROPERTY(meta = (BindWidget))
		UTextBlock*	Text_ItemCount;

	// ������ �ε��� ��ȣ
	int SlotIndex;


protected:

	// �ش� ������ �巡�� �̺�Ʈ�� �߻����� ��� ȣ��˴ϴ�.
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	// ��Ŭ���� ������ ���� �κ��丮�� ������ �����մϴ�.
	virtual void SlotChange() override;

	// �巡�װ� ������ ��� ȣ��Ǵ� �޼���
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;


public:
	UInventorySlot(const FObjectInitializer& ObjectInitializer);

	// ������ ������ �̹����� ������Ʈ�մϴ�.
	virtual void UpdateItemImage(int itemCode, int itemCount = 1) override;

	// �κ��丮 ���Գ����� ������ ��ȯ�մϴ�.
	void SwapInvFromInventorySlot(UInventorySlot* swapInventorySlot);

	// ��񽽷԰� �κ��丮 ���Գ����� ������ ��ȯ�մϴ�.
	void SwapInvFromEquipSlot(class UEquipItemSlot* swapEquipSlot);

	// �ڵ�� ������ ������ ������ �����մϴ�.
	void SetItemInfoFromCode(int itemCode);	

	// ���� �ε����� �����մϴ�.
	void SetSlotIndex(int index);

	FORCEINLINE int GetSlotIndex() const
	{
		return SlotIndex;
	}

};
