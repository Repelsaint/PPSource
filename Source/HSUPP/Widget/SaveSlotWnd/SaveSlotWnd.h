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
	// 슬롯들이 생성되서 위치할 스크롤박스입니다.
	UPROPERTY(meta = (BindWidget))
		class UScrollBox* ScrollBox_SaveSlotBox;

	// 캐릭터 선택 / 삭제 버튼입니다.
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Select;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Delete;

	// 슬롯들을 가지고있을 배열입니다.
	TArray<class USaveSlot*> SaveSlotArray;
	
	// 슬롯의 블루프린트입니다.
	TSubclassOf<class USaveSlot> BP_SaveSlot;

	// 선택된 슬롯 인덱스를 구분하기 위한 변수입니다.
	int SelectedSlotIndex;

public:
	USaveSlotWnd(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

	// 선택된 슬롯이 바뀌었을때 호출될 메서드입니다.
	 void ChangeSelectedSlot(int index);


	 // 슬롯을 생성합니다.
	 void CreateSaveSlots();

	 // 게임을 시작하는 메서드입니다.
	 UFUNCTION()
	 void StartGame();

	 // 캐릭터 삭제를 할것인지 확인하는 메서드입니다.
	 UFUNCTION()
	 void TryDeleteCharacter();

	 // 캐릭터를 삭제합니다.
	 void DeleteCharacter();

	FORCEINLINE int GetSelectedSlotIndex() const
	{ return SelectedSlotIndex; }
};
