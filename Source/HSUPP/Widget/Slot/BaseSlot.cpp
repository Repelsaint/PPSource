// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSlot.h"
#include "Struct/ItemInfo.h"
#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"
#include "Components/CanvasPanelSlot.h"


UBaseSlot::UBaseSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// 빈 이미지 입니다.
	static ConstructorHelpers::FObjectFinder<UTexture2D> NONE_IMAGE(TEXT("Texture2D'/Game/Resources/Slot/T_NULL.T_NULL'"));
	if (NONE_IMAGE.Succeeded())
		NoneImage = NONE_IMAGE.Object;

	// 아이템의 정보들을 가진 데이터테이블을 로드합니다.
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ITEMINFO(TEXT("DataTable'/Game/DataTable/DT_ItemInfo.DT_ItemInfo'"));
	if (DT_ITEMINFO.Succeeded())
		DT_ItemInfo = DT_ITEMINFO.Object;
	
	// 드래그 슬롯 블루프린트를 가져옵니다.
	static ConstructorHelpers::FClassFinder<UUserWidget> BP_DRAG_SLOT(TEXT("WidgetBlueprint'/Game/Widget/PlayerInventory/BP_DragSlot.BP_DragSlot_C'"));
	if(BP_DRAG_SLOT.Succeeded())
		BP_DragSlot = BP_DRAG_SLOT.Class;
	
	// 드래깅 상태를 초기화합니다.
	bIsDragging = false;
}

void UBaseSlot::UpdateItemImage(int itemCode, int itemCount)
{
	if (itemCode != 0)
	{
		//코드가 존재하는 경우 코드에 맞는 이미지를 넣어줍니다.
		FName ItemRowName = FName(*FString::FromInt(itemCode));
		UTexture2D* newItemImage = Cast<UTexture2D>(GetStreamManager()->LoadSynchronous
		(DT_ItemInfo->FindRow<FItemInfo>(ItemRowName, TEXT(""))->ItemIconPath));
		Image_Item->SetBrushFromTexture(newItemImage);
	}
	else
		//아이템 코드가 없을경우 빈칸이미지를 넣어줍니다.
		Image_Item->SetBrushFromTexture(NoneImage);
}





FReply UBaseSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	//어떤 버튼이 눌려졌는지를 검사합니다.
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		// 색을 클릭용 색으로 바꿔줍니다.
		SetSlotColorClicked();
		// 드래그 앤 드롭 작업을 시작하며, 작업 결과를 반환합니다.
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	else if(InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{	
		//우측클릭이 되었다면 슬롯 체인지 메서드를 호출합니다.
		SlotChange();
	}

	return FReply::Handled();
}

FReply UBaseSlot::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry,InMouseEvent);

	//클릭이 종료 되었으므로 호버상태로 변경합니다.
	SetSlotColorHover();
	return FReply::Unhandled();
}



void UBaseSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	//드래그드랍오퍼레이션으로 지정할 객체를 만듭니다.
	USlotDragOperation* dragDropOperation = Cast<USlotDragOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(USlotDragOperation::StaticClass()));
	bIsDragging = true;

	// 드래그중인 오리진슬롯을 이 슬롯으로 만듭니다.
	dragDropOperation->OriginSlot = this;

	// 드래그시 이미지를 띄울 위젯을 생성합니다.
	UUserWidget* dragSlot = CreateWidget<UUserWidget>(this,BP_DragSlot);

	// 드래그 슬롯의 이미지를 설정할 위젯을 이름으로 찾아서 설정해줍니다.
	Cast<UImage>(dragSlot->GetWidgetFromName(TEXT("Image_Drag")))->SetBrushFromTexture(Cast<UTexture2D>(Image_Item->Brush.GetResourceObject()));
	dragDropOperation->DefaultDragVisual = dragSlot;

	// 아웃 오퍼레이션을 생성한 드래그드롭 오퍼레이션으로 지정해줍니다.
	OutOperation = dragDropOperation;

}

void UBaseSlot::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
	//드래그 실패시 색을 원상복구합니다.
	bIsDragging = false;
	SetSlotColorNormal();
}

bool UBaseSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	// 드랍결과를 저장합니다.
	auto Value = Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	
	// 드래그 상태를 원상복구합니다.
	bIsDragging = false;

	// 드래그가 끝났으므로 색을 원상복귀합니다.
	Cast<USlotDragOperation>(InOperation)->OriginSlot->SetSlotColorNormal();

	return Value;
}

void UBaseSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry,InMouseEvent);

	//마우스 커서가 칸에 들어가면 색을 변경해줍니다.
	SetSlotColorHover();
}

void UBaseSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	//칸을 벗어났을때 드래그중이 아니라면 색을 원상복구합니다.
	if (!bIsDragging)
	{
		SetSlotColorNormal();
	}
}


void UBaseSlot::SetSlotColorNormal()
{	
	//노말 상태의 색으로 지정합니다.
	Image_BackGround->SetBrushTintColor(FLinearColor(1.0f, 1.0f, 1.0f));
}

void UBaseSlot::SetSlotColorHover()
{	
	// 슬롯에 아이콘이 올려져있을때의 색상입니다.
	Image_BackGround->SetBrushTintColor(FLinearColor(0.8f, 0.8f, 0.8f));
}

void UBaseSlot::SetSlotColorClicked()
{	
	// 아이템이 클릭되었을때의 색상입니다.
	Image_BackGround->SetBrushTintColor(FLinearColor(0.2f, 0.2f, 0.2f));
}

void UBaseSlot::SetSlotType(ESlotType slotType)
{
	//슬롯의 타입을 지정합니다.
	SlotType = slotType;
}

void UBaseSlot::SlotChange()
{
}

