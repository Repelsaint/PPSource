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
	// ������ ���� �ؽ�Ʈ�Դϴ�.
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextBlock_Level;
	// ������ ĳ���� �̸� �ؽ�Ʈ �Դϴ�.
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextBlock_CharacterName;
	
	// �� ������ ��� ������ �̹��� �����Դϴ�.
	UPROPERTY(meta = (BindWidget))
		class UImage* Image_EmptySlot;

	// ���� Ŭ���� ��ȣ�ۿ�� ��ư�Դϴ�.
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Select;

	// ������ ���� �ε���
	int SlotIndex;
	// �� ������ ��� �� �ƴҰ�� ��Ÿ�� �̹����Դϴ�.
	class UTexture2D* NullImage;
	class UTexture2D* EmptySlotImage;

	// ������ ����ִ����� ��Ÿ���ϴ�.
	bool bEmpty;
	// ������ ���� Ȥ�� ���� �����϶��� ���Դϴ�.
	FLinearColor PressedColor;
	FLinearColor NormalColor;


public:
	USaveSlot(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

	// ������ �ʱ�ȭ�մϴ�.
	void InitializeSaveSlot();
	
	// ������ Ŭ�������� ȣ��˴ϴ�.
	UFUNCTION()
	void SelectSlot();

	// ������ ������� ���������ϴ�.
	void SetNormalColor();

	
	FORCEINLINE void SetSlotIndex(int newSlotIndex)
	{ SlotIndex = newSlotIndex; }
	FORCEINLINE int GetSlotIndex() const
	{ return SlotIndex; }
	
};
