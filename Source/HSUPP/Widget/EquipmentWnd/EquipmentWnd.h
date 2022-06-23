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
	// 장비슬롯들의 배열입니다.
	TMap<EItemType,UEquipItemSlot*> EquipSlots;

	// 캐릭터 인포텍스트입니다.
	class UTextBlock* CharacterInfoText;

	
public:
	UEquipmentWnd(const FObjectInitializer& objectinitializer);

	// 장비창을 초기화합니다.
	void InitializeEquipWnd();

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// 장비가 변경된 경우 정보를 바꿔줍니다.
	void UpdateEquipInfo(EItemType itemType, int value);

	FORCEINLINE UEquipItemSlot* GetEquipSlot(EItemType itemType)
	{
		return EquipSlots[itemType];
	}
};
