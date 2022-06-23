// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultWndBase.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"

#include "Widget/MainWidget.h"
#include "Widget/HomeWidget/HomeWidget.h"


UDefaultWndBase::UDefaultWndBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//창의 기본 크기를 지정합니다.
	WndSize = FVector2D(600.0f, 600.0f);
	// 드래그 상태를 초기화합니다.
	bDraggable = false;
	
	// 기본 가시성을 켜줍니다.
	Visibility = ESlateVisibility::Visible;
}


void UDefaultWndBase::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼이 눌렀을때 호출 될 메서드를 바인딩합니다.
	Button_Close->OnClicked.AddDynamic(this, &UDefaultWndBase::CloseThisWnd);
	Button_DraggableBar->OnPressed.AddDynamic(this, &UDefaultWndBase::WndDragStart);
	Button_DraggableBar->OnReleased.AddDynamic(this, &UDefaultWndBase::WndDragEnded);

	// 창의 원점과 화면상의 원점을 가운데로 지정합니다.
	Cast<UCanvasPanelSlot>(this->Slot)->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
	Cast<UCanvasPanelSlot>(this->Slot)->SetAlignment(FVector2D(0.5f, 0.5f));
}

void UDefaultWndBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 윈도우 드래그를 호출합니다.
	DraggingWnd();
}

FReply UDefaultWndBase::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// 창을 클릭하면 Reply값을 Handled로 반환합니다.
	// 빈칸을 눌러도 공격이 나가지 않도록 하기 위함입니다.
	Super::NativeOnMouseButtonDown(InGeometry,InMouseEvent);
	return FReply::Handled();
}



void UDefaultWndBase::CloseThisWnd()
{
	// 메인 위젯이 nullptr이면 홈위젯의 창을 끕니다.
	if (MainWidget != nullptr)
	{
		MainWidget->CloseWnd(this);
	}
	else
	{
		GetHInstance()->GetHomeWidget()->CloseWnd(this);
	}
}

void UDefaultWndBase::WndDragStart()
{
	//윈도우의 드래그 상태를 참으로 바꿔줍니다.
	bDraggable = true;

	// 메인 위젯의 포커스중인 창을 현재창으로 바꿔줍니다.
	MainWidget->SetFocusWnd(this);

	// 드래그를 시작하면 이전 드래그 포지션을 저장합니다.
	PrevDragPos = Cast<UCanvasPanelSlot>(this->Slot)->GetPosition();

	// 드래그 시작시 현재의 마우스 위치를 저장합니다.
	PrevMPosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);

}

void UDefaultWndBase::WndDragEnded()
{
	//드래그상태를 거짓으로 저장합니다.
	bDraggable = false;
}

void UDefaultWndBase::SetMainWidget(UMainWidget* mainWidget)
{
	MainWidget = mainWidget;
}

void UDefaultWndBase::DraggingWnd()
{
	// 드래그중이 아니라면 실행하지않습니다.
	if (!bDraggable) return;

	// 현재의 마우스 위치값을 얻어옵니다.
	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);

	// 윈도우의 슬롯을 얻어옵니다.
	UCanvasPanelSlot* DragWnd = Cast<UCanvasPanelSlot>(this->Slot);

	// 슬롯의 포지션을 마우스가 이동한 만큼 옮겨줍니다.
	DragWnd->SetPosition((PrevDragPos + ( MousePosition - PrevMPosition)));

	// 옮겨준 위치로 이전 창 위치와 마우스 위치를 저장합니다.
	PrevDragPos = DragWnd->GetPosition();
	PrevMPosition = MousePosition;
}
