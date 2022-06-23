// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSUPP.h"
#include "Blueprint/UserWidget.h"
#include "SaveSlot.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API USaveSlot : public UUserWidget
{
	GENERATED_BODY()

private:
	// 슬롯의 레벨 텍스트입니다.
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextBlock_Level;
	// 슬롯의 캐릭터 이름 텍스트 입니다.
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextBlock_CharacterName;
	
	// 빈 슬롯일 경우 보여질 이미지 슬롯입니다.
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_EmptySlot;

	// 슬롯 클릭시 상호작용될 버튼입니다.
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Select;

	// 슬롯이 가진 인덱스
	int SlotIndex;
	// 빈 슬롯일 경우 와 아닐경우 나타날 이미지입니다.
	class UTexture2D* NullImage;
	class UTexture2D* EmptySlotImage;

	// 슬롯이 비어있는지를 나타냅니다.
	bool bEmpty;
	// 슬롯이 선택 혹은 비선택 상태일때의 색입니다.
	FLinearColor PressedColor;
	FLinearColor NormalColor;


public:
	USaveSlot(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

	// 슬롯을 초기화합니다.
	void InitializeSaveSlot();
	
	// 슬롯을 클릭했을때 호출됩니다.
	UFUNCTION()
	void SelectSlot();

	// 색깔을 원래대로 돌려놓습니다.
	void SetNormalColor();

	
	FORCEINLINE void SetSlotIndex(int newSlotIndex)
	{ SlotIndex = newSlotIndex; }
	FORCEINLINE int GetSlotIndex() const
	{ return SlotIndex; }
	
};
