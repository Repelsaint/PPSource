// Fill out your copyright notice in the Description page of Project Settings.


#include "HomeWidget.h"

#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

#include "Widget/NewGameBox/NewGameBox.h"
#include "Widget/DefaultWndBase.h"
#include "Widget/NoticeBox/NoticeBox.h"
#include "Widget/SaveSlotWnd/SaveSlotWnd.h"
#include "Widget/SelectCheckBox/SelectCheckBox.h"
#include "Widget/OptionWnd/OptionWnd.h"

#include "Actor/Controller/CharacterController.h"

#include "Kismet/KismetSystemLibrary.h"

UHomeWidget::UHomeWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// 새게임 버튼을 눌렀을떄 나올  창을 불러옵니다.
	static ConstructorHelpers::FClassFinder<UNewGameBox> BP_NEWGAMEBOX(TEXT("WidgetBlueprint'/Game/Widget/NewGameBox/BP_NewGameBox.BP_NewGameBox_C'"));
	if(BP_NEWGAMEBOX.Succeeded())
		BP_NewGameBox = BP_NEWGAMEBOX.Class;

	// 에러창이 뜰떄 사용할 창을 불러옵니다.
	static ConstructorHelpers::FClassFinder<UNoticeBox> BP_NOTICE_BOX(TEXT("WidgetBlueprint'/Game/Widget/NoticeBox/BP_NoticeBox.BP_NoticeBox_C'"));
	if (BP_NOTICE_BOX.Succeeded())
		BP_NoticeBox = BP_NOTICE_BOX.Class;

	// 불러오기 버튼을 눌렀을때 나타날 창입니다.
	static ConstructorHelpers::FClassFinder<USaveSlotWnd> BP_SAVE_WND(TEXT("WidgetBlueprint'/Game/Widget/SaveSlotWnd/BP_SaveSlotWNd.BP_SaveSlotWnd_C'"));
	if(BP_SAVE_WND.Succeeded())
		BP_SaveSlotWnd = BP_SAVE_WND.Class;

	// 선택시 체크를 위해 사용될 창입니다.
	static ConstructorHelpers::FClassFinder<USelectCheckBox> SELECT_CHECK_BOX(TEXT("WidgetBlueprint'/Game/Widget/CheckBox/BP_SelectCheckBox.BP_SelectCheckBox_C'"));
	if (SELECT_CHECK_BOX.Succeeded())
		BP_SelectCheckBox = SELECT_CHECK_BOX.Class;

	// 옵션 버튼을 눌렀을때 나올 창을 불러옵니다.
	static ConstructorHelpers::FClassFinder<UOptionWnd> BP_OPTION_WND(TEXT("WidgetBlueprint'/Game/Widget/Setting/BP_SettingWnd.BP_SettingWnd_C'"));
	if (BP_OPTION_WND.Succeeded())
		BP_OptionWnd = BP_OPTION_WND.Class;

}

void UHomeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 각 버튼을 눌렀을때 호출이 되어야하는 메서드를 바인딩합니다.
	Button_NewGame->OnClicked.AddDynamic(this, &UHomeWidget::PressNewGameButton);
	Button_ContinueGame->OnClicked.AddDynamic(this, &UHomeWidget::PressContinueButton);
	Button_Setting->OnClicked.AddDynamic(this, &UHomeWidget::PressSettingButton);
	Button_Exit->OnClicked.AddDynamic(this, &UHomeWidget::PressExitButton);
}

void UHomeWidget::PressNewGameButton()
{
	// 새 게임을 위한 창을 생성합니다.
	NewGameBox = CreateWidget<UNewGameBox>(this, BP_NewGameBox);
	NewGameBox->OnWndClosed.AddLambda([this]()
	{
		NewGameBox = nullptr;
	});
	CanvasPanel_WndParent->AddChild(NewGameBox);	
	
}

void UHomeWidget::PressContinueButton()
{
	// 세이브슬롯창을 생성합니다.
	SaveSlotWnd = CreateWidget<USaveSlotWnd>(this, BP_SaveSlotWnd);
	SaveSlotWnd->OnWndClosed.AddLambda([this]()
		{
			SaveSlotWnd = nullptr;
		});
	CanvasPanel_WndParent->AddChild(SaveSlotWnd);
}

void UHomeWidget::PressSettingButton()
{
	//옵션 창을 생성하고옵션창 크기를 지정합니다.
	OptionWnd = CreateWidget<UOptionWnd>(this, BP_OptionWnd);
	OptionWnd->OnWndClosed.AddLambda([this]()
	{
		OptionWnd = nullptr;
	});
	
	CanvasPanel_WndParent->AddChild(OptionWnd);
	Cast<UCanvasPanelSlot>(OptionWnd->Slot)->SetSize(OptionWnd->GetWndSize());
	
}

void UHomeWidget::PressExitButton()
{
	// 게임을 종료합니다.
	UKismetSystemLibrary::QuitGame(GetWorld(),GetHInstance()->GetPlayerController(), EQuitPreference::Quit,true);
}

void UHomeWidget::CloseWnd(UDefaultWndBase* defaultWnd)
{
	// 바인딩된 메서드를 호출하고 패널에서 제거해줍니다.
	defaultWnd->OnWndClosed.Broadcast();
	CanvasPanel_WndParent->RemoveChild(defaultWnd);

}

USelectCheckBox* UHomeWidget::CreateCheckBox(FText messageText)
{
	// 선택을 확인하는 창을 생성합니다.
	SelectCheckBox = CreateWidget<USelectCheckBox>(this, BP_SelectCheckBox);

	SelectCheckBox->OnWndClosed.AddLambda([this]()
		{
			SelectCheckBox = nullptr;
		});

	CanvasPanel_WndParent->AddChild(SelectCheckBox);
	// 지정하고자하는 메세지로 텍스트를 변경합니다.
	SelectCheckBox->SetMessageText(messageText);


	return SelectCheckBox;
}

void UHomeWidget::CreateNoticeBox(FText titleText, FText messageText)
{
	// 알림창을 생성합니다.
	NoticeBox = CreateWidget<UNoticeBox>(this, BP_NoticeBox);
	// 필요한 제목과 메시지로 값을 변경해줍니다.
	NoticeBox->SetTitleText(titleText);
	NoticeBox->SetMessageText(messageText);
	NoticeBox->OnWndClosed.AddLambda([this]()
	{
		NoticeBox = nullptr;
	});
	CanvasPanel_WndParent->AddChild(NoticeBox);
	// 창 크기를 조정해줍니다.
	Cast<UCanvasPanelSlot>(NoticeBox->Slot)->SetSize(NoticeBox->GetWndSize());

	
}

