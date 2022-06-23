// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/DefaultWndBase.h"
#include "Enum/ItemType.h"
#include "Widget/Slot/EquipSlot/EquipItemSlot.h"

#include "EquipmentWnd.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API UEquipmentWnd : public UDefaultWndBase
{
	GENERATED_BODY()

private:
	// ��񽽷Ե��� �迭�Դϴ�.
	TMap<EItemType,UEquipItemSlot*> EquipSlots;

	// ĳ���� �����ؽ�Ʈ�Դϴ�.
	class UTextBlock* CharacterInfoText;

	
public:
	UEquipmentWnd(const FObjectInitializer& objectinitializer);

	// ���â�� �ʱ�ȭ�մϴ�.
	void InitializeEquipWnd();

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// ��� ����� ��� ������ �ٲ��ݴϴ�.
	void UpdateEquipInfo(EItemType itemType, int value);

	FORCEINLINE UEquipItemSlot* GetEquipSlot(EItemType itemType)
	{
		return EquipSlots[itemType];
	}
};
