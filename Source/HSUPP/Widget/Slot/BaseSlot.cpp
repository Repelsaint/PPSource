// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSlot.h"
#include "Struct/ItemInfo.h"
#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"
#include "Components/CanvasPanelSlot.h"


UBaseSlot::UBaseSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// �� �̹��� �Դϴ�.
	static ConstructorHelpers::FObjectFinder<UTexture2D> NONE_IMAGE(TEXT("Texture2D'/Game/Resources/Slot/T_NULL.T_NULL'"));
	if (NONE_IMAGE.Succeeded())
		NoneImage = NONE_IMAGE.Object;

	// �������� �������� ���� ���������̺��� �ε��մϴ�.
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ITEMINFO(TEXT("DataTable'/Game/DataTable/DT_ItemInfo.DT_ItemInfo'"));
	if (DT_ITEMINFO.Succeeded())
		DT_ItemInfo = DT_ITEMINFO.Object;
	
	// �巡�� ���� �������Ʈ�� �����ɴϴ�.
	static ConstructorHelpers::FClassFinder<UUserWidget> BP_DRAG_SLOT(TEXT("WidgetBlueprint'/Game/Widget/PlayerInventory/BP_DragSlot.BP_DragSlot_C'"));
	if(BP_DRAG_SLOT.Succeeded())
		BP_DragSlot = BP_DRAG_SLOT.Class;
	
	// �巡�� ���¸� �ʱ�ȭ�մϴ�.
	bIsDragging = false;
}

void UBaseSlot::UpdateItemImage(int itemCode, int itemCount)
{
	if (itemCode != 0)
	{
		//�ڵ尡 �����ϴ� ��� �ڵ忡 �´� �̹����� �־��ݴϴ�.
		FName ItemRowName = FName(*FString::FromInt(itemCode));
		UTexture2D* newItemImage = Cast<UTexture2D>(GetStreamManager()->LoadSynchronous
		(DT_ItemInfo->FindRow<FItemInfo>(ItemRowName, TEXT(""))->ItemIconPath));
		Image_Item->SetBrushFromTexture(newItemImage);
	}
	else
		//������ �ڵ尡 ������� ��ĭ�̹����� �־��ݴϴ�.
		Image_Item->SetBrushFromTexture(NoneImage);
}





FReply UBaseSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	//� ��ư�� ������������ �˻��մϴ�.
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		// ���� Ŭ���� ������ �ٲ��ݴϴ�.
		SetSlotColorClicked();
		// �巡�� �� ��� �۾��� �����ϸ�, �۾� ����� ��ȯ�մϴ�.
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	else if(InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{	
		//����Ŭ���� �Ǿ��ٸ� ���� ü���� �޼��带 ȣ���մϴ�.
		SlotChange();
	}

	return FReply::Handled();
}

FReply UBaseSlot::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry,InMouseEvent);

	//Ŭ���� ���� �Ǿ����Ƿ� ȣ�����·� �����մϴ�.
	SetSlotColorHover();
	return FReply::Unhandled();
}



void UBaseSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	//�巡�׵�����۷��̼����� ������ ��ü�� ����ϴ�.
	USlotDragOperation* dragDropOperation = Cast<USlotDragOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(USlotDragOperation::StaticClass()));
	bIsDragging = true;

	// �巡������ ������������ �� �������� ����ϴ�.
	dragDropOperation->OriginSlot = this;

	// �巡�׽� �̹����� ��� ������ �����մϴ�.
	UUserWidget* dragSlot = CreateWidget<UUserWidget>(this,BP_DragSlot);

	// �巡�� ������ �̹����� ������ ������ �̸����� ã�Ƽ� �������ݴϴ�.
	Cast<UImage>(dragSlot->GetWidgetFromName(TEXT("Image_Drag")))->SetBrushFromTexture(Cast<UTexture2D>(Image_Item->Brush.GetResourceObject()));
	dragDropOperation->DefaultDragVisual = dragSlot;

	// �ƿ� ���۷��̼��� ������ �巡�׵�� ���۷��̼����� �������ݴϴ�.
	OutOperation = dragDropOperation;

}

void UBaseSlot::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
	//�巡�� ���н� ���� ���󺹱��մϴ�.
	bIsDragging = false;
	SetSlotColorNormal();
}

bool UBaseSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	// �������� �����մϴ�.
	auto Value = Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	
	// �巡�� ���¸� ���󺹱��մϴ�.
	bIsDragging = false;

	// �巡�װ� �������Ƿ� ���� ���󺹱��մϴ�.
	Cast<USlotDragOperation>(InOperation)->OriginSlot->SetSlotColorNormal();

	return Value;
}

void UBaseSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry,InMouseEvent);

	//���콺 Ŀ���� ĭ�� ���� ���� �������ݴϴ�.
	SetSlotColorHover();
}

void UBaseSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	//ĭ�� ������� �巡������ �ƴ϶�� ���� ���󺹱��մϴ�.
	if (!bIsDragging)
	{
		SetSlotColorNormal();
	}
}


void UBaseSlot::SetSlotColorNormal()
{	
	//�븻 ������ ������ �����մϴ�.
	Image_BackGround->SetBrushTintColor(FLinearColor(1.0f, 1.0f, 1.0f));
}

void UBaseSlot::SetSlotColorHover()
{	
	// ���Կ� �������� �÷����������� �����Դϴ�.
	Image_BackGround->SetBrushTintColor(FLinearColor(0.8f, 0.8f, 0.8f));
}

void UBaseSlot::SetSlotColorClicked()
{	
	// �������� Ŭ���Ǿ������� �����Դϴ�.
	Image_BackGround->SetBrushTintColor(FLinearColor(0.2f, 0.2f, 0.2f));
}

void UBaseSlot::SetSlotType(ESlotType slotType)
{
	//������ Ÿ���� �����մϴ�.
	SlotType = slotType;
}

void UBaseSlot::SlotChange()
{
}

