// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/DefaultWndBase.h"
#include "SaveSlotWnd.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API USaveSlotWnd : public UDefaultWndBase
{
	GENERATED_BODY()

private:
	// ���Ե��� �����Ǽ� ��ġ�� ��ũ�ѹڽ��Դϴ�.
	UPROPERTY(meta = (BindWidget))
		class UScrollBox* ScrollBox_SaveSlotBox;

	// ĳ���� ���� / ���� ��ư�Դϴ�.
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Select;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Delete;

	// ���Ե��� ���������� �迭�Դϴ�.
	TArray<class USaveSlot*> SaveSlotArray;
	
	// ������ �������Ʈ�Դϴ�.
	TSubclassOf<class USaveSlot> BP_SaveSlot;

	// ���õ� ���� �ε����� �����ϱ� ���� �����Դϴ�.
	int SelectedSlotIndex;

public:
	USaveSlotWnd(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

	// ���õ� ������ �ٲ������ ȣ��� �޼����Դϴ�.
	 void ChangeSelectedSlot(int index);


	 // ������ �����մϴ�.
	 void CreateSaveSlots();

	 // ������ �����ϴ� �޼����Դϴ�.
	 UFUNCTION()
	 void StartGame();

	 // ĳ���� ������ �Ұ����� Ȯ���ϴ� �޼����Դϴ�.
	 UFUNCTION()
	 void TryDeleteCharacter();

	 // ĳ���͸� �����մϴ�.
	 void DeleteCharacter();

	FORCEINLINE int GetSelectedSlotIndex() const
	{ return SelectedSlotIndex; }
};
