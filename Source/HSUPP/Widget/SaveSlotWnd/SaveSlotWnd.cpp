// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveSlotWnd.h"

#include "Widget/Slot/SaveSlot/SaveSlot.h"
#include "Widget/SelectCheckBox/SelectCheckBox.h"
#include "Widget/HomeWidget/HomeWidget.h"

#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"


USaveSlotWnd::USaveSlotWnd(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// 세이브 칸 블루프린트를 불러옵니다.
	static ConstructorHelpers::FClassFinder<USaveSlot> SAVE_SLOT_WIDGET(TEXT("WidgetBlueprint'/Game/Widget/SaveSlotWnd/BP_SaveSlot.BP_SaveSlot_C'"));
	if(SAVE_SLOT_WIDGET.Succeeded())
		BP_SaveSlot = SAVE_SLOT_WIDGET.Class;
}

void USaveSlotWnd::NativeConstruct()
{
	Super::NativeConstruct();

	// 창의 크기를 지정합니다.
	Cast<UCanvasPanelSlot>(Slot)->SetSize(FVector2D(1200.0f,700.0f));

	// 타이틀의 이름을 지정합니다.
	SetTitleText(TEXT("Load File"));

	// 선택된 창의 인덱스를 초기화합니다.
	SelectedSlotIndex -= 1;

	// 세이브 슬롯을 생성합니다.
	CreateSaveSlots();

	// 버튼을 눌렀을때 호출될 메서드를 바인딩합니다.
	Button_Select->OnClicked.AddDynamic(this,&USaveSlotWnd::StartGame);
	Button_Delete->OnClicked.AddDynamic(this,&USaveSlotWnd::TryDeleteCharacter);
		
}

void USaveSlotWnd::ChangeSelectedSlot(int index)
{
	// 제대로된 창을 눌렀다면 기존의 슬롯의 색을 원래대로 돌려줍니다.
	if(SelectedSlotIndex != -1)
		SaveSlotArray[SelectedSlotIndex]->SetNormalColor();
	if (SelectedSlotIndex == index)
	{
		//같은 슬롯을 눌렀다면 선택을 취소합니다.
		SelectedSlotIndex = -1;
	}
	else SelectedSlotIndex = index;
}

void USaveSlotWnd::CreateSaveSlots()
{
	// 최대 슬롯 갯수를 읽어옵니다.
	int maxSlotNum = GetHInstance()->GetMaxSlotNum();
	for (int i = 0; i < maxSlotNum; ++i)
	{
		// 슬롯위젯을  새로 생성합니다.
		USaveSlot* newSlotWidget = CreateWidget<USaveSlot>(this, BP_SaveSlot);
		// 생성한 위젯을 스크롤박스에 붙여줍니다.
		ScrollBox_SaveSlotBox->AddChild(newSlotWidget);

		if (i == (maxSlotNum - 1))
		{
			// 지금 생성한 슬롯이 마지막이라면 아래쪽 패딩을 따로 지정해줍니다.
			Cast<UScrollBoxSlot>(newSlotWidget->Slot)->SetPadding(FMargin(5.0f, 10.0f, 5.0f, 10.0f));
		}
		else
		{
			// 슬롯의 패딩을 지정해줍니다.
			Cast<UScrollBoxSlot>(newSlotWidget->Slot)->SetPadding(FMargin(5.0f, 10.0f, 5.0f, 0.0f));
		}

		//생성된 슬롯의 인덱스를 지정하고 초기화해줍니다.
		newSlotWidget->SetSlotIndex(i);
		newSlotWidget->InitializeSaveSlot();

		// 슬롯을 배열에 추가합니다.
		SaveSlotArray.Add(newSlotWidget);
	}
}

void USaveSlotWnd::StartGame()
{
	// 캐릭터를 선택했을때 제대로 선택이 되어있지 않다면 실행하지 않습니다.
	if(SelectedSlotIndex == -1) return;

	// 선택된 인덱스로 데이터 정보를 읽어옵니다.
	GetHInstance()->LoadData(SelectedSlotIndex);
	//플레이중인 캐릭터의 인덱스를 설정합니다.
	GetHInstance()->SetCurrentPlayerIndex(SelectedSlotIndex);

	// 마을 맵을 엽니다.
	GetHInstance()->OpenNewMap(FName(TEXT("VilageLevel")));
}

void USaveSlotWnd::DeleteCharacter()
{
	// 선택된 인덱스의 파일을 제거합니다.
	GetHInstance()->DeleteSaveFile(SelectedSlotIndex);
	
	// 제거된 창의 정보를 초기화합니다.
	SaveSlotArray[SelectedSlotIndex]->InitializeSaveSlot();

	// 삭제가 되었으므로 선택을 취소합니다.
	SelectedSlotIndex = -1;
}

void USaveSlotWnd::TryDeleteCharacter()
{
	// 캐릭터를 선택했을때 제대로 선택이 되어있지 않다면 실행하지 않습니다.
	if (SelectedSlotIndex == -1) return;

	// 삭제버튼을 누를때 확인할 수 있게 메세지를 띄웁니다.
	FText newText = FText::FromString(TEXT("캐릭터를 삭제 하시겠습니까?"));

	//체크박스를 생성하고 확인을 눌렀을때 캐릭터를 삭제할수 있게 메서드를 바운딩합니다.
	GetHInstance()->GetHomeWidget()->CreateCheckBox(newText)->OnMessageBoxChecked.AddLambda([this]()
	{	
		DeleteCharacter();
	});
	
}
