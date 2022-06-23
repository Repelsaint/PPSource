// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSUPP.h"

#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Blueprint/WidgetBlueprintLibrary.h"	
#include "Widget/Slot/SlotDragOperation/SlotDragOperation.h"
#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"

#include "Enum/SlotType.h"

#include "BaseSlot.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API UBaseSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	//아이템과 배경의 이미지입니다.
	UPROPERTY(meta = (BindWidget))
		UImage* Image_BackGround;
	UPROPERTY(meta = (BindWidget))
		UImage* Image_Item;

		//드래그슬롯 블루프린트입니다.
		TSubclassOf<UUserWidget> BP_DragSlot;
	
	// 비어있는 이미지입니다.
	UTexture2D* NoneImage;

	// 드래그상태를 나타냅니다.
	bool bIsDragging;

	//생성된 슬롯의 타입을 나타냅니다.
	ESlotType SlotType;

	// 아이템들의 실제 데이터(이미지경로/최대수량 등)
	class UDataTable* DT_ItemInfo;


protected:

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override;


	// 해당 위젯에 드래그 이벤트가 발생했을 경우 호출됩니다.
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	// 드래그중 드래그 취소 시 호출되는 메서드
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	// 드래그가 끝났을 경우 호출되는 메서드
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	// 마우스가 올려졌을때 호출될 메서드
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	//마우스가 칸을 벗어났을때 호출될 메서드
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);




public:
	UBaseSlot(const FObjectInitializer& ObjectInitializer);

	// 아이템의 코드와 수량에 맞게끔 이미지를 업데이트합니다.
	virtual void UpdateItemImage(int itemCode, int itemCount = 1);

	// 클릭상태에 따른 색상을 지정합니다.
	void SetSlotColorNormal();
	void SetSlotColorHover();
	void SetSlotColorClicked();

	// 슬롯의 타입을 지정합니다.
	void SetSlotType(ESlotType slotType);

	// 슬롯을 서로 교환합니다.
	virtual void SlotChange();

	FORCEINLINE ESlotType GetSlotType() const
	{ return SlotType; }
};
