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
	//�κ��丮 ���Ե��� ������ ��ġ�Դϴ�.
	UPROPERTY(meta = (BindWidget))
		class UGridPanel* GridPanel_SlotParent;

		// �����ݾ��� ǥ���� ��ġ�Դϴ�.
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Money;


	// �κ��丮 ������ �������Ʈ�Դϴ�.
	TSubclassOf<class UInventorySlot> BP_InventorySlot;

	// ���Ե��� ����� �迭�Դϴ�.
	TArray<UInventorySlot*> SlotArray;


private:
	//�κ��丮�� �ʱ�ȭ�մϴ�.
	void IntializeInventoryWindow();

protected:

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UInventoryWnd(const FObjectInitializer& ObjectInitializer);	

	//�κ��丮������ ��� �ʱ�ȭ�մϴ�.
	void UpdateAllInventorySlot();

	FORCEINLINE UInventorySlot* GetInventorySlot(int indexNum)
	{ return SlotArray[indexNum]; }
};
