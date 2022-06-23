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
	// 이 장비칸이 어떤 부위의 장비칸인지를 나타냅니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EItemType EquipSlotType;


protected:

	// 해당 위젯에 드래그 이벤트가 발생했을 경우 호출됩니다.
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	// 장비칸을 우클릭을 하여 장비칸을 비울때 호출이 됩니다.
	virtual void SlotChange() override;


	// 드래그가 끝났을 경우 호출되는 메서드
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;


public:
	
	// 인벤토리 슬롯에서 장비칸으로 스왑할때 호출됩니다.
	void SwapEquipFromInvSlot(class UInventorySlot* invSlot);


	FORCEINLINE EItemType GetSlotEquipType() const
	{ return EquipSlotType; }

	
};
