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
	//â�� �⺻ ũ�⸦ �����մϴ�.
	WndSize = FVector2D(600.0f, 600.0f);
	// �巡�� ���¸� �ʱ�ȭ�մϴ�.
	bDraggable = false;
	
	// �⺻ ���ü��� ���ݴϴ�.
	Visibility = ESlateVisibility::Visible;
}


void UDefaultWndBase::NativeConstruct()
{
	Super::NativeConstruct();

	// ��ư�� �������� ȣ�� �� �޼��带 ���ε��մϴ�.
	Button_Close->OnClicked.AddDynamic(this, &UDefaultWndBase::CloseThisWnd);
	Button_DraggableBar->OnPressed.AddDynamic(this, &UDefaultWndBase::WndDragStart);
	Button_DraggableBar->OnReleased.AddDynamic(this, &UDefaultWndBase::WndDragEnded);

	// â�� ������ ȭ����� ������ ����� �����մϴ�.
	Cast<UCanvasPanelSlot>(this->Slot)->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
	Cast<UCanvasPanelSlot>(this->Slot)->SetAlignment(FVector2D(0.5f, 0.5f));
}

void UDefaultWndBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// ������ �巡�׸� ȣ���մϴ�.
	DraggingWnd();
}

FReply UDefaultWndBase::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// â�� Ŭ���ϸ� Reply���� Handled�� ��ȯ�մϴ�.
	// ��ĭ�� ������ ������ ������ �ʵ��� �ϱ� �����Դϴ�.
	Super::NativeOnMouseButtonDown(InGeometry,InMouseEvent);
	return FReply::Handled();
}



void UDefaultWndBase::CloseThisWnd()
{
	// ���� ������ nullptr�̸� Ȩ������ â�� ���ϴ�.
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
	//�������� �巡�� ���¸� ������ �ٲ��ݴϴ�.
	bDraggable = true;

	// ���� ������ ��Ŀ������ â�� ����â���� �ٲ��ݴϴ�.
	MainWidget->SetFocusWnd(this);

	// �巡�׸� �����ϸ� ���� �巡�� �������� �����մϴ�.
	PrevDragPos = Cast<UCanvasPanelSlot>(this->Slot)->GetPosition();

	// �巡�� ���۽� ������ ���콺 ��ġ�� �����մϴ�.
	PrevMPosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);

}

void UDefaultWndBase::WndDragEnded()
{
	//�巡�׻��¸� �������� �����մϴ�.
	bDraggable = false;
}

void UDefaultWndBase::SetMainWidget(UMainWidget* mainWidget)
{
	MainWidget = mainWidget;
}

void UDefaultWndBase::DraggingWnd()
{
	// �巡������ �ƴ϶�� ���������ʽ��ϴ�.
	if (!bDraggable) return;

	// ������ ���콺 ��ġ���� ���ɴϴ�.
	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);

	// �������� ������ ���ɴϴ�.
	UCanvasPanelSlot* DragWnd = Cast<UCanvasPanelSlot>(this->Slot);

	// ������ �������� ���콺�� �̵��� ��ŭ �Ű��ݴϴ�.
	DragWnd->SetPosition((PrevDragPos + ( MousePosition - PrevMPosition)));

	// �Ű��� ��ġ�� ���� â ��ġ�� ���콺 ��ġ�� �����մϴ�.
	PrevDragPos = DragWnd->GetPosition();
	PrevMPosition = MousePosition;
}
