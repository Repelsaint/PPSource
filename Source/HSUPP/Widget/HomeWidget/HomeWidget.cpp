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
	// ������ ��ư�� �������� ����  â�� �ҷ��ɴϴ�.
	static ConstructorHelpers::FClassFinder<UNewGameBox> BP_NEWGAMEBOX(TEXT("WidgetBlueprint'/Game/Widget/NewGameBox/BP_NewGameBox.BP_NewGameBox_C'"));
	if(BP_NEWGAMEBOX.Succeeded())
		BP_NewGameBox = BP_NEWGAMEBOX.Class;

	// ����â�� �㋚ ����� â�� �ҷ��ɴϴ�.
	static ConstructorHelpers::FClassFinder<UNoticeBox> BP_NOTICE_BOX(TEXT("WidgetBlueprint'/Game/Widget/NoticeBox/BP_NoticeBox.BP_NoticeBox_C'"));
	if (BP_NOTICE_BOX.Succeeded())
		BP_NoticeBox = BP_NOTICE_BOX.Class;

	// �ҷ����� ��ư�� �������� ��Ÿ�� â�Դϴ�.
	static ConstructorHelpers::FClassFinder<USaveSlotWnd> BP_SAVE_WND(TEXT("WidgetBlueprint'/Game/Widget/SaveSlotWnd/BP_SaveSlotWNd.BP_SaveSlotWnd_C'"));
	if(BP_SAVE_WND.Succeeded())
		BP_SaveSlotWnd = BP_SAVE_WND.Class;

	// ���ý� üũ�� ���� ���� â�Դϴ�.
	static ConstructorHelpers::FClassFinder<USelectCheckBox> SELECT_CHECK_BOX(TEXT("WidgetBlueprint'/Game/Widget/CheckBox/BP_SelectCheckBox.BP_SelectCheckBox_C'"));
	if (SELECT_CHECK_BOX.Succeeded())
		BP_SelectCheckBox = SELECT_CHECK_BOX.Class;

	// �ɼ� ��ư�� �������� ���� â�� �ҷ��ɴϴ�.
	static ConstructorHelpers::FClassFinder<UOptionWnd> BP_OPTION_WND(TEXT("WidgetBlueprint'/Game/Widget/Setting/BP_SettingWnd.BP_SettingWnd_C'"));
	if (BP_OPTION_WND.Succeeded())
		BP_OptionWnd = BP_OPTION_WND.Class;

}

void UHomeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// �� ��ư�� �������� ȣ���� �Ǿ���ϴ� �޼��带 ���ε��մϴ�.
	Button_NewGame->OnClicked.AddDynamic(this, &UHomeWidget::PressNewGameButton);
	Button_ContinueGame->OnClicked.AddDynamic(this, &UHomeWidget::PressContinueButton);
	Button_Setting->OnClicked.AddDynamic(this, &UHomeWidget::PressSettingButton);
	Button_Exit->OnClicked.AddDynamic(this, &UHomeWidget::PressExitButton);
}

void UHomeWidget::PressNewGameButton()
{
	// �� ������ ���� â�� �����մϴ�.
	NewGameBox = CreateWidget<UNewGameBox>(this, BP_NewGameBox);
	NewGameBox->OnWndClosed.AddLambda([this]()
	{
		NewGameBox = nullptr;
	});
	CanvasPanel_WndParent->AddChild(NewGameBox);	
	
}

void UHomeWidget::PressContinueButton()
{
	// ���̺꽽��â�� �����մϴ�.
	SaveSlotWnd = CreateWidget<USaveSlotWnd>(this, BP_SaveSlotWnd);
	SaveSlotWnd->OnWndClosed.AddLambda([this]()
		{
			SaveSlotWnd = nullptr;
		});
	CanvasPanel_WndParent->AddChild(SaveSlotWnd);
}

void UHomeWidget::PressSettingButton()
{
	//�ɼ� â�� �����ϰ�ɼ�â ũ�⸦ �����մϴ�.
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
	// ������ �����մϴ�.
	UKismetSystemLibrary::QuitGame(GetWorld(),GetHInstance()->GetPlayerController(), EQuitPreference::Quit,true);
}

void UHomeWidget::CloseWnd(UDefaultWndBase* defaultWnd)
{
	// ���ε��� �޼��带 ȣ���ϰ� �гο��� �������ݴϴ�.
	defaultWnd->OnWndClosed.Broadcast();
	CanvasPanel_WndParent->RemoveChild(defaultWnd);

}

USelectCheckBox* UHomeWidget::CreateCheckBox(FText messageText)
{
	// ������ Ȯ���ϴ� â�� �����մϴ�.
	SelectCheckBox = CreateWidget<USelectCheckBox>(this, BP_SelectCheckBox);

	SelectCheckBox->OnWndClosed.AddLambda([this]()
		{
			SelectCheckBox = nullptr;
		});

	CanvasPanel_WndParent->AddChild(SelectCheckBox);
	// �����ϰ����ϴ� �޼����� �ؽ�Ʈ�� �����մϴ�.
	SelectCheckBox->SetMessageText(messageText);


	return SelectCheckBox;
}

void UHomeWidget::CreateNoticeBox(FText titleText, FText messageText)
{
	// �˸�â�� �����մϴ�.
	NoticeBox = CreateWidget<UNoticeBox>(this, BP_NoticeBox);
	// �ʿ��� ����� �޽����� ���� �������ݴϴ�.
	NoticeBox->SetTitleText(titleText);
	NoticeBox->SetMessageText(messageText);
	NoticeBox->OnWndClosed.AddLambda([this]()
	{
		NoticeBox = nullptr;
	});
	CanvasPanel_WndParent->AddChild(NoticeBox);
	// â ũ�⸦ �������ݴϴ�.
	Cast<UCanvasPanelSlot>(NoticeBox->Slot)->SetSize(NoticeBox->GetWndSize());

	
}

