// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSUPP.h"
#include "Blueprint/UserWidget.h"

#include "Components/Textblock.h"

#include "DefaultWndBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnWndClosedEvent)
/**
 * 
 */
UCLASS()
class HSUPP_API UDefaultWndBase : public UUserWidget
{
	GENERATED_BODY()

private:
	// 닫기 버튼입니다.
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Close;
	// 창의 이름을 나타내는 텍스트입니다.
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Title;
	// 드래그를 편리하게 하기 위한 버튼입니다.
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_DraggableBar;

	// 드래그 상태를 나타냅니다.
	bool bDraggable;
	//  이전 마우스 포지션의 위치입니다.
	struct FVector2D PrevMPosition;

public:
	// 창이 닫힐때 호출 될 대리자입니다.
	FOnWndClosedEvent OnWndClosed;
	
protected:
	// 메인위젯입니다.
	UPROPERTY()
		class UMainWidget* MainWidget;

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	// 창의 빈 위치를 클릭했을때 호출됩니다.
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

protected :
	// 창의 크기를 나타냅니다.
	FVector2D WndSize;
	// 이전 창의 위치입니다.
	FVector2D PrevDragPos;

public:
	UDefaultWndBase(const FObjectInitializer& ObjectInitializer);

public:

	// 현재 창을 닫을 때 호출됩니다.
	UFUNCTION()
	void CloseThisWnd();

	// 윈도우 드래그를 시작 / 끝낼때 호출됩니다.
	UFUNCTION()
	void WndDragStart();
	UFUNCTION()
	void WndDragEnded();

	void SetMainWidget(class UMainWidget* mainWidget);
	
	//창을 드래그합니다.
	void DraggingWnd();


	FORCEINLINE FVector2D GetWndSize() const
	{ return WndSize; }

	FORCEINLINE void SetTitleText(FString titleText)
	{
		Text_Title->SetText(FText::FromString(titleText));
	}
	FORCEINLINE void SetTitleText(FText titleText)
	{
		Text_Title->SetText(titleText);
	}
};
