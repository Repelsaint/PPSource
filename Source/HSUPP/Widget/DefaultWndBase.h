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
	// �ݱ� ��ư�Դϴ�.
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Close;
	// â�� �̸��� ��Ÿ���� �ؽ�Ʈ�Դϴ�.
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_Title;
	// �巡�׸� ���ϰ� �ϱ� ���� ��ư�Դϴ�.
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_DraggableBar;

	// �巡�� ���¸� ��Ÿ���ϴ�.
	bool bDraggable;
	//  ���� ���콺 �������� ��ġ�Դϴ�.
	struct FVector2D PrevMPosition;

public:
	// â�� ������ ȣ�� �� �븮���Դϴ�.
	FOnWndClosedEvent OnWndClosed;
	
protected:
	// ���������Դϴ�.
	UPROPERTY()
		class UMainWidget* MainWidget;

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	// â�� �� ��ġ�� Ŭ�������� ȣ��˴ϴ�.
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

protected :
	// â�� ũ�⸦ ��Ÿ���ϴ�.
	FVector2D WndSize;
	// ���� â�� ��ġ�Դϴ�.
	FVector2D PrevDragPos;

public:
	UDefaultWndBase(const FObjectInitializer& ObjectInitializer);

public:

	// ���� â�� ���� �� ȣ��˴ϴ�.
	UFUNCTION()
	void CloseThisWnd();

	// ������ �巡�׸� ���� / ������ ȣ��˴ϴ�.
	UFUNCTION()
	void WndDragStart();
	UFUNCTION()
	void WndDragEnded();

	void SetMainWidget(class UMainWidget* mainWidget);
	
	//â�� �巡���մϴ�.
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
