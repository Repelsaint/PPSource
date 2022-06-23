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
	//�����۰� ����� �̹����Դϴ�.
	UPROPERTY(meta = (BindWidget))
		UImage* Image_BackGround;
	UPROPERTY(meta = (BindWidget))
		UImage* Image_Item;

		//�巡�׽��� �������Ʈ�Դϴ�.
		TSubclassOf<UUserWidget> BP_DragSlot;
	
	// ����ִ� �̹����Դϴ�.
	UTexture2D* NoneImage;

	// �巡�׻��¸� ��Ÿ���ϴ�.
	bool bIsDragging;

	//������ ������ Ÿ���� ��Ÿ���ϴ�.
	ESlotType SlotType;

	// �����۵��� ���� ������(�̹������/�ִ���� ��)
	class UDataTable* DT_ItemInfo;


protected:

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override;


	// �ش� ������ �巡�� �̺�Ʈ�� �߻����� ��� ȣ��˴ϴ�.
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	// �巡���� �巡�� ��� �� ȣ��Ǵ� �޼���
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	// �巡�װ� ������ ��� ȣ��Ǵ� �޼���
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	// ���콺�� �÷������� ȣ��� �޼���
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	//���콺�� ĭ�� ������� ȣ��� �޼���
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);




public:
	UBaseSlot(const FObjectInitializer& ObjectInitializer);

	// �������� �ڵ�� ������ �°Բ� �̹����� ������Ʈ�մϴ�.
	virtual void UpdateItemImage(int itemCode, int itemCount = 1);

	// Ŭ�����¿� ���� ������ �����մϴ�.
	void SetSlotColorNormal();
	void SetSlotColorHover();
	void SetSlotColorClicked();

	// ������ Ÿ���� �����մϴ�.
	void SetSlotType(ESlotType slotType);

	// ������ ���� ��ȯ�մϴ�.
	virtual void SlotChange();

	FORCEINLINE ESlotType GetSlotType() const
	{ return SlotType; }
};
