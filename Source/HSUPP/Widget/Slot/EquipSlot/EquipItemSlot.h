// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/Slot/BaseSlot.h"
#include "Enum/ItemType.h"

#include "EquipItemSlot.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API UEquipItemSlot : public UBaseSlot
{
	GENERATED_BODY()

private:
	// �� ���ĭ�� � ������ ���ĭ������ ��Ÿ���ϴ�.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EItemType EquipSlotType;


protected:

	// �ش� ������ �巡�� �̺�Ʈ�� �߻����� ��� ȣ��˴ϴ�.
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	// ���ĭ�� ��Ŭ���� �Ͽ� ���ĭ�� ��ﶧ ȣ���� �˴ϴ�.
	virtual void SlotChange() override;


	// �巡�װ� ������ ��� ȣ��Ǵ� �޼���
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;


public:
	
	// �κ��丮 ���Կ��� ���ĭ���� �����Ҷ� ȣ��˴ϴ�.
	void SwapEquipFromInvSlot(class UInventorySlot* invSlot);


	FORCEINLINE EItemType GetSlotEquipType() const
	{ return EquipSlotType; }

	
};
