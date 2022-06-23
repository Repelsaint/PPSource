// Fill out your copyright notice in the Description page of Project Settings.

#include "MainWidget.h"

#include "Actor/Character/BaseNpc/BaseNpc.h"

#include "Engine/Engine.h"

#include "Widget/DefaultWndbase.h"
#include "Widget/InventoryWnd/InventoryWnd.h"
#include "Widget/EquipmentWnd/EquipmentWnd.h"
#include "Widget/OptionWnd/OptionWnd.h"
#include "Widget/InteractableMessage/InteractableMessage.h"
#include "Widget/NpcDialog/NpcDialog.h"
#include "Widget/NpcShopWnd/NpcShopWnd.h"
#include "Widget/TradeCheckBox/TradeCheckBox.h"
#include "Widget/NoticeBox/NoticeBox.h"

#include "Single/GameSettings/GameSettings.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

UMainWidget::UMainWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
	// 인벤토리 블루프린트입니다.
	static ConstructorHelpers::FClassFinder<UInventoryWnd> BP_INVENTORY_WND(TEXT("WidgetBlueprint'/Game/Widget/PlayerInventory/BP_InventoryWnd.BP_InventoryWnd_C'"));
	if (BP_INVENTORY_WND.Succeeded())
		BP_InventoryWnd = BP_INVENTORY_WND.Class;
	
	// 장비창 블루프린트입니다.
	static ConstructorHelpers::FClassFinder<UEquipmentWnd> BP_EQUIPOMENT_WND(TEXT("WidgetBlueprint'/Game/Widget/PlayerInventory/BP_EquipWnd.BP_EquipWnd_C'"));
	if (BP_EQUIPOMENT_WND.Succeeded())
		BP_EquipmentWnd = BP_EQUIPOMENT_WND.Class;

	// 옵션 창 블루프린트입니다.
	static ConstructorHelpers::FClassFinder<UOptionWnd> BP_OPTION_WND(TEXT("WidgetBlueprint'/Game/Widget/Setting/BP_SettingWnd.BP_SettingWnd_C'"));
	if (BP_OPTION_WND.Succeeded())
		BP_OptionWnd = BP_OPTION_WND.Class;

	// 상점 창 블루프린트입니다.
	static ConstructorHelpers::FClassFinder<UNpcShopWnd> BP_SHOP_WND(TEXT("WidgetBlueprint'/Game/Widget/NpcFunction/BP_NpcShop.BP_NpcShop_C'"));
	if(BP_SHOP_WND.Succeeded())
		BP_ShopWnd = BP_SHOP_WND.Class;

	// 상호작용 메세지 블루프린트입니다.
	static ConstructorHelpers::FClassFinder<UInteractableMessage> BP_INTERACTABLE_MESSAGE(TEXT("WidgetBlueprint'/Game/Widget/Interact/BP_Interactable.BP_Interactable_C'"));
	if (BP_INTERACTABLE_MESSAGE.Succeeded())
		BP_InteractableMessage = BP_INTERACTABLE_MESSAGE.Class;

	//트레이드 체크박스 블루프린트입니다.
	static ConstructorHelpers::FClassFinder<UTradeCheckBox> BP_TRADE_CHECKBOX(TEXT("WidgetBlueprint'/Game/Widget/CheckBox/BP_TradeCheckBox.BP_TradeCheckBox_C'"));
	if(BP_TRADE_CHECKBOX.Succeeded())
		BP_TradeCheckBox = BP_TRADE_CHECKBOX.Class;

	// 알림창 블루프린트입니다.
	static ConstructorHelpers::FClassFinder<UNoticeBox> BP_NOTICE_BOX(TEXT("WidgetBlueprint'/Game/Widget/NoticeBox/BP_NoticeBox.BP_NoticeBox_C'"));
	if(BP_NOTICE_BOX.Succeeded())
		BP_NoticeBox = BP_NOTICE_BOX.Class;

	// 창들의 기본 이전윈도우 창의 위치를 저장합니다.
	PrevWndPos.Add(TEXT("BP_EquipWnd_C"), FVector2D(-200.0f, 0.0f));
	PrevWndPos.Add(TEXT("BP_InventoryWnd_C"), FVector2D(200.0f, 0.0f));
	PrevWndPos.Add(TEXT("BP_SettingWnd_C"), FVector2D(0.0f, 0.0f));

}

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// 메인 위젯을 초기화합니다.
	InitializeMainWidget();
}

void UMainWidget::InitializeMainWidget()
{
	// 각 bool값을 모두 초기화 합니다.
	bInteract = bOptionWndClosing = bTrade = false;

	// 유저의 정보를 읽어와서 HPBar와 정보 텍스트를 설정합니다.
	FPlayerInfo& playerInfo = GetHInstance()->GetPlayerCharacter()->GetPlayerInfo();
	SetHPBar(playerInfo.CurrentHP, playerInfo.MaxHP);
	SetCharacterDataText(playerInfo.Level,playerInfo.Exp,playerInfo.CharacterName);
}

void UMainWidget::SetCharacterDataText(int level, int exp, FName characterName)
{
	// 캐릭터 데이터를 FText로 변환하고 적용시킵니다.
	FText newText = FText::FromString(characterName.ToString());
	newText = FText::Format(FText::FromString(TEXT("Lv. {0} ({1}%)   {2}")),level, ((float)(exp*100) / (level *10)), newText);
	TextBlock_CharacterData->SetText(newText);
}

UDefaultWndBase* UMainWidget::CreateNewWnd(TSubclassOf<UDefaultWndBase> wndClass, bool bUsePrevPos)
{
	// DefaultWndBase형식으로 새창을 생성합니다.
	UDefaultWndBase* newWnd = CreateWidget<UDefaultWndBase>(this, wndClass);

	// 생성된 창을 지정된 부모에 붙입니다.
	CanvasPanel_WindowParent->AddChild(newWnd);

	if (WndArray.Num() == 0)
	{
		//첫 생성되는 윈도우라면 인풋모드를 변경해주고 마우스가 보이게 설정합니다.
		PlayerController->SetInputMode(FInputModeGameAndUI());
		PlayerController->bShowMouseCursor = true;
		// 창이 켜진 동안에는 줌을 할수 없도록 설정합니다.
		GetHInstance()->GetPlayerCharacter()->GetSpringArm()->SetUseZoom(false);
	}

	// 이전 창 위치를 사용하는 창이라면 이전 위치로 그렇지 않다면 중앙에 소환해줍니다.
	if (bUsePrevPos)
	{
		Cast<UCanvasPanelSlot>(newWnd->Slot)->SetPosition(PrevWndPos[newWnd->GetClass()->GetName()]);
	}
	else
		Cast<UCanvasPanelSlot>(newWnd->Slot)->SetPosition(FVector2D(0.0f,0.0f));

	// 창의 기본 크기를 설정합니다.
	Cast<UCanvasPanelSlot>(newWnd->Slot)->SetSize(newWnd->GetWndSize());

	// 배열에 창을 추가합니다.
	WndArray.Add(newWnd);

	// 창의 메인위젯을 설정해주고 반환합니다.
	newWnd->SetMainWidget(this);
	return newWnd;
}


void UMainWidget::CloseWnd(UDefaultWndBase* defaultWnd)
{
	// 창의 포지션을 저장합니다.
	SaveWndPos(defaultWnd);

	// 창을 닫을 때 호출될 메서드를 실행시킵니다.
	defaultWnd->OnWndClosed.Broadcast();
	
	// 부모객체로부터 떼어냅니다.
	CanvasPanel_WindowParent->RemoveChild(defaultWnd);

	//배열에서 창을 제거해주고 게임모드를 리셋해줍니다.
	WndArray.Remove(defaultWnd);
	ResetGameMode();

}

void UMainWidget::CreateInventoryWnd()
{
	
	if (!IsValid(InventoryWnd))
	{
	// 생성된 인벤토리 창이 없다면 생성해줍니다.
		InventoryWnd = Cast<UInventoryWnd>(CreateNewWnd(BP_InventoryWnd, true));
		
		// 인벤토리 창이 꺼질때 스스로를 nullptr로 바꾸게끔 설정해줍니다.
		InventoryWnd->OnWndClosed.AddLambda([this]()
			{
				InventoryWnd = nullptr;
			});
	}
	else
	{
		// 이미 창이 켜져있다면 인벤토리창을 닫습니다.
		InventoryWnd->CloseThisWnd();
	}

	
}

void UMainWidget::CreateEquipmentWnd()
{
	if (!IsValid(EquipWnd))
	{
		// 장비창이 생성 되어 있지 않다면 장비창을 생성해줍니다.
		EquipWnd = Cast<UEquipmentWnd>(CreateNewWnd(BP_EquipmentWnd, true));
		// 장비 창을 닫으면 창을 nullptr로 지정해줍니다.
		EquipWnd->OnWndClosed.AddLambda([this]()
			{
				EquipWnd = nullptr;
			});
	}
	else
	{
		//창이 열려있다면 닫아줍니다.
		EquipWnd->CloseThisWnd();
	}

}

void UMainWidget::CreateOptionWnd()
{
	
	if (!IsValid(OptionWnd))
	{
		//bool값을 초기화해줍니다.
		bOptionWndClosing = false;
		//옵션창을 생성합니다.
		OptionWnd = Cast<UOptionWnd>(CreateNewWnd(BP_OptionWnd, false));

		OptionWnd->OnWndClosed.AddLambda([this]()
		{
			//	이미 닫히고 있는 중이라면 다시 닫지 않도록 합니다.
			if(bOptionWndClosing) return;
			// 옵션창이 닫히면 bool값을 변경해주고 옵션을 저장합니다.
			bOptionWndClosing = true;
			OptionWnd->SaveAllOptions();
			OptionWnd = nullptr;
		});
	}
	else
	{
		//옵션창을 닫습니다.
		OptionWnd->CloseThisWnd();
	}
	
}

void UMainWidget::CreateNewShopWnd(int npcCode)
{
	//이미 창이 열려있다면 실행하지 않습니다.
	if(NpcShopWnd != nullptr) return;

	// npc상점과 유저의 인벤토리 창을 생성해줍니다.
	NpcShopWnd = Cast<UNpcShopWnd>(CreateNewWnd(BP_ShopWnd, false));

	if(InventoryWnd == nullptr)
	{
		InventoryWnd = Cast<UInventoryWnd>(CreateNewWnd(BP_InventoryWnd, false));
		Cast<UCanvasPanelSlot>(InventoryWnd->Slot)->SetPosition(FVector2D(210.0f,0.0f));
	}

	// Npc 상점을 초기화 해주고 위치를 초기화합니다.
	NpcShopWnd->InitShopItemInfo(npcCode);
	NpcShopWnd->InitShopWndPos();
	
	// npc 창이 닫힐때 호출될 메서드입니다.
	NpcShopWnd->OnWndClosed.AddLambda([this]()
		{
			NpcShopWnd = nullptr;
			// 상점을 닫으면 인벤토리도 같이 닫습니다.
			if (InventoryWnd != nullptr)
				CloseWnd(InventoryWnd);
		});

	InventoryWnd->OnWndClosed.AddLambda([this]()
		{
			InventoryWnd = nullptr;
			// 인벤토리를 닫으면 상점도 같이 닫습니다.
			if(NpcShopWnd != nullptr)
				CloseWnd(NpcShopWnd);
		});

}

void UMainWidget::SetPlayerController(ACharacterController* playerController)
{
	PlayerController = playerController;
}

void UMainWidget::ResetGameMode()
{
	// 창이 존재하지 않는다면 게임모드를 원래대로 돌려줍니다.
	if (WndArray.Num() == 0 && DialogWidget == nullptr)
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
		GetHInstance()->GetPlayerCharacter()->GetSpringArm()->SetUseZoom(true);
	}
}

void UMainWidget::SetFocusWnd(UDefaultWndBase* focusWnd)
{
	// 거래가 진행중이라면 창을 클릭해도 포커싱이 바뀌지 않습니다.
	if(bTrade) return;

	// 지금 열린 창이 포커스되어있는지를 검사합니다.
	if (CanvasPanel_WindowParent->GetChildIndex(focusWnd) != CanvasPanel_WindowParent->GetSlots().Num() - 1)
	{
		// 포커스 되어있지않다면 재등록하여 포커스를 잡아줍니다.
		
		// 창의 이전 위치등의 정보를 저장합니다.
		UCanvasPanelSlot* FocusWndSlot = Cast<UCanvasPanelSlot>(focusWnd->Slot);
		FVector2D SlotAlignment = FocusWndSlot->GetAlignment();
		FAnchors SlotAnchors = FocusWndSlot->GetAnchors();
		FVector2D LastPosition = FocusWndSlot->GetPosition();

		// 배열에서 창을 빼줍니다.
		WndArray.Remove(focusWnd);
		// 포커스 하려는 창을 재등록해줍니다.
		// 이미 등록된 창을 다시 AddChild로 하면 해제후 다시 등록이 됩니다.
		CanvasPanel_WindowParent->AddChild(focusWnd);

		// 포커스 창이 재등록 되었으므로 슬롯을 다시 받아옵니다.
		FocusWndSlot = Cast<UCanvasPanelSlot>(focusWnd->Slot);

		// 창을 배열에 추가해줍니다.
		WndArray.Add(focusWnd);

		// 저장해두었던 값을 다시 적용해줍니다.
		FocusWndSlot->SetAnchors(SlotAnchors);
		FocusWndSlot->SetAlignment(SlotAlignment);
		FocusWndSlot->SetPosition(LastPosition);
		FocusWndSlot->SetSize(focusWnd->GetWndSize());
	}

}

void UMainWidget::CloseFocusWnd()
{
	// 가장 마지막에 추가된 포커스된 창을 닫습니다.
	CloseWnd(WndArray[WndArray.Num() - 1]);
}

void UMainWidget::SaveWndPos(UDefaultWndBase* saveWnd)
{
	// 닫을 창의 이름을 FString 형식으로 받아옵니다.
	FString newClosewnd = saveWnd->GetClass()->GetName();
	// 창의 이전위치값을 처음 저장한다면 배열에 새로 추가하여 줍니다.
	if(!PrevWndPos.Contains(newClosewnd))
		PrevWndPos.Add(saveWnd->GetClass()->GetName(),FVector2D());
	// 이전 창의 위치를 저장합니다.
	PrevWndPos[saveWnd->GetClass()->GetName()] = Cast<UCanvasPanelSlot>(saveWnd->Slot)->GetPosition();
}

void UMainWidget::SetHPBar(int currentHP, int maxHP)
{
	// HPBar 에 들어온 정보를 토대로 텍스트를 변경합니다.
	FText newText;
	newText = FText::Format(FText::FromString(TEXT("{0}/{1}")), currentHP, maxHP);
	Text_HP->SetText(newText);

	if (maxHP != 0)
	{	
		// 현재 체력의  퍼센트만큼 프로그레스바의 퍼센트를 변경해줍니다.
		ProgressBar_HP->SetPercent( ((float)currentHP / maxHP) );
	}
}

void UMainWidget::CreateInteractMessage(ABaseNpc* interactNpcc)
{
	if (InteractableMessage == nullptr)
	{
		//상호작용 가능 메세지가 없다면 생성해줍니다.
		InteractableMessage = CreateWidget<UInteractableMessage>(this, BP_InteractableMessage);
		// 메세지의 위치를 결정할 NPC와 컨트롤러를 지정합니다.
		InteractableMessage->SetTargetNpc(interactNpcc);
		InteractableMessage->SetPlayerController(PlayerController);

		// 위젯부모객체에 붙여줍니다.
		CanvasPanel_WidgetParent->AddChild(InteractableMessage);
		
		// 슬롯을 얻어와서 기본 크기와 원점을 지정합니다.
		UCanvasPanelSlot* messagePanel = Cast<UCanvasPanelSlot>(InteractableMessage->Slot);
		messagePanel->SetSize(FVector2D(200.0f, 50.0f));
		messagePanel->SetAnchors(FAnchors(0.0f, 0.0f));
		messagePanel->SetAlignment(FVector2D(0.5f, 0.5f));
		// 현재 설정된 DPI값을 초기화 합니다.
		OnScreenOptionChanged();
	}
}


void UMainWidget::DeleteInteractMessage(ABaseNpc* interactNpcc)
{
	// 위젯 부모객체에서 상호작용가능 메세지를 제거하고
	CanvasPanel_WidgetParent->RemoveChild(InteractableMessage);

	// 메세지를 비웁니다.
	InteractableMessage->SetTargetNpc(nullptr);
	InteractableMessage = nullptr;
}

void UMainWidget::OnScreenOptionChanged()
{
	// 메세지가 생성되어있지않다면 실행하지않습니다.
	if(InteractableMessage != nullptr)
	{
		//현재 스크린 모드를 검사합니다.
		if (GetHInstance()->GetGameSetting()->GetScreenMode() == EWindowMode::WindowedFullscreen ||
			GetHInstance()->GetGameSetting()->GetScreenMode() == EWindowMode::Fullscreen)
		{
			//전체화면이라면 디스플레이의 해상도를 읽어옵니다.
			FIntPoint screenResolution = GetHInstance()->GetGameSetting()->GetFullScreenResolution();
			// DPI를 디스플레이의 해상도를 이용하여 지정합니다.
			InteractableMessage->SetDPI(FVector2D((1920.0f / screenResolution.X), (1080.0f / ((int)screenResolution.Y))));
		}
		else
		{
			//창모드라면 현재 게임 화면의 해상도에 맞게 DPI를 설정하여줍니다.
			FVector2D newScreenResolution = GetHInstance()->GetGameSetting()->GetScreenResolution();
			InteractableMessage->SetDPI(FVector2D((1920 / newScreenResolution.X), (1080 / newScreenResolution.Y)));
		}
	}
}

void UMainWidget::AddChildWidget(UUserWidget* newChildWidget)
{
	// 위젯을 자식으로 갖습니다.
	CanvasPanel_WidgetParent->AddChild(newChildWidget);
}

void UMainWidget::CreateDialog(UNpcDialog* newDialog)
{
	//위젯 부모객체에 다이얼로그를 붙입니다.
	CanvasPanel_WidgetParent->AddChild(Cast<UUserWidget>(newDialog));
	// 상호작용가능 메세지를 숨깁니다.
	InteractableMessage->SetVisibility(ESlateVisibility::Hidden);
	// 상호작용중임을 나타내고 다이얼로그를 들어온 변수로 설정합니다.
	bInteract = true;
	DialogWidget = newDialog;

	// 상호작용이 실행되었으므로 모든 창을 닫아줍니다.
	CloseAllWnd();
	
	newDialog->OnWidgetClosed.AddLambda([this]()
	{
		//다이얼로그가 닫히면 상호작용중임을 거짓으로 바꿉니다.
		bInteract = false;
		// 숨겨두었던 상호작용 가능 메세지를 다시 보이게 설정합니다.
		InteractableMessage->SetVisibility(ESlateVisibility::Visible);
		// 다이얼로그 위젯을 패널에서 제거합니다.
		CanvasPanel_WidgetParent->RemoveChild(DialogWidget);
	}
	);
}

void UMainWidget::CreateTradeCheckBox(int itemCode,int maxItemCount,int itemCost, int& currentMoney)
{
	// 트레이드 체크박스를 생성합니다.
	TradeCheckBox = Cast<UTradeCheckBox>(CreateNewWnd(BP_TradeCheckBox, false));

	TradeCheckBox->OnWndClosed.AddLambda([this]()
		{
		// 창이 닫히면 거래중 상태를 거짓으로 바꾸고 포인터를 초기화합니다.
			bTrade = false;
			TradeCheckBox = nullptr;
		});
	// 거래중상태를 나타냅니다.
	bTrade = true;

	// 창의 슬롯의 사이즈와 원점을 초기화합니다.
	Cast<UCanvasPanelSlot>(TradeCheckBox->Slot)->SetSize(FVector2D(400.0f,200.0f));
	Cast<UCanvasPanelSlot>(TradeCheckBox->Slot)->SetAnchors(FAnchors(0.5f,0.5f));
	Cast<UCanvasPanelSlot>(TradeCheckBox->Slot)->SetAlignment(FVector2D(0.5f,0.5f));

	//변수를 통하여 창의 메세지를 변경합니다.
	TradeCheckBox->InitTradeBox(itemCode, maxItemCount, itemCost,currentMoney);
}

void UMainWidget::CreateNoticeBox(FText titleText, FText messageText)
{
	// 이미 알림창이 생성되어있다면 생성하지않습니다.
	if(NoticeBox != nullptr) return;

	// 알림상자를 생성합니다.
	NoticeBox = Cast<UNoticeBox>(CreateNewWnd(BP_NoticeBox, false));

	
	NoticeBox->OnWndClosed.AddLambda([this]()
	{
		// 포인터를 비웁니다.
		NoticeBox = nullptr;
	});
	// 상자의 제목과 내용을 지정합니다.
	NoticeBox->SetTitleText(titleText);
	NoticeBox->SetMessageText(messageText);
}

void UMainWidget::CloseAllWnd()
{
	//배열에 들어있는 모든 창을 닫습니다.
	for (int i = 0; i < WndArray.Num(); ++i)
	{
		CloseWnd(WndArray[i]);
	}
}

void UMainWidget::ClosedDialogWidget()
{
	// 상점이 열려있다면 닫지 않습니다.
	if (NpcShopWnd != nullptr) return;

	// 바인딩되어있던 메서드를 호출하고 창을 비웁니다.
	DialogWidget->OnWidgetClosed.Broadcast();
	DialogWidget = nullptr;

	// 게임모드를 리셋합니다.
	ResetGameMode();
}
