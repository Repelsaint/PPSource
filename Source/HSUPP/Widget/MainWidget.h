// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSUPP.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameUserSettings.h"
#include "Actor/Controller/CharacterController.h"
#include "Widget/EquipmentWnd/EquipmentWnd.h"

#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	//플레이어 컨트롤러를 나타냅니다.
	ACharacterController* PlayerController;

	//각 UI들을 소유할 부모 객체들입니다.
	UPROPERTY(meta = (BindWidget))
		class UCanvasPanel* CanvasPanel_WidgetParent;
	UPROPERTY(meta = (BindWidget))
		class UCanvasPanel* CanvasPanel_WindowParent;
	UPROPERTY(meta = (BindWidget))
		class UCanvasPanel* CanvasPanel_UIParent;

	// 캐릭터의 레벨과 이름을 나타낼 텍스트입니다.
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextBlock_CharacterData;
	
	// 플레이어의 체력을 나타내는 창입니다.
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Text_HP;

	// 플레이어의 체력퍼센트를 나타내는 바입니다.
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* ProgressBar_HP;

	// 각 창들의 블루프린트입니다.
	TSubclassOf<class UInventoryWnd> BP_InventoryWnd;
	TSubclassOf<class UEquipmentWnd> BP_EquipmentWnd;
	TSubclassOf<class UOptionWnd> BP_OptionWnd;
	TSubclassOf<class UNpcShopWnd> BP_ShopWnd;
	TSubclassOf<class UInteractableMessage> BP_InteractableMessage;
	TSubclassOf<class UTradeCheckBox> BP_TradeCheckBox;
	TSubclassOf<class UNoticeBox> BP_NoticeBox;

	//창들을 저장할 배열
	TArray<class UDefaultWndBase*> WndArray;
	
	// 창들의 이전 위치를 저장하는 배열입니다.
	TMap<FString, FVector2D> PrevWndPos;

	// 각 창들을 가리킬 포인터입니다.
	class UInventoryWnd* InventoryWnd;
	class UEquipmentWnd* EquipWnd;
	class UOptionWnd* OptionWnd;
	class UInteractableMessage* InteractableMessage;
	class UNpcDialog* DialogWidget;
	class UNpcShopWnd* NpcShopWnd;
	class UTradeCheckBox* TradeCheckBox;
	class UNoticeBox* NoticeBox;

	// 옵션창 종료후 빠른 생성시 버그발생억제용
	bool bOptionWndClosing;

	// 현재 플레이어의 상호작용 거래 상태를 나타냅니다.
	bool bInteract;
	bool bTrade;

public:

	UMainWidget(const FObjectInitializer& ObjectInitializer);

public:
	virtual void NativeConstruct() override;
	
	// 메인위젯을 초기화할때 사용합니다.
	void InitializeMainWidget();

	// 캐릭터 데이터 텍스트를 지정합니다.
	void SetCharacterDataText(int level, int exp, FName characterName);

	// 새 창을 생성할때 사용되는 메서드입니다.
	class UDefaultWndBase* CreateNewWnd(TSubclassOf<UDefaultWndBase> wndClass, bool bUsePrevPos = false);

	// 창을 닫을 때 호출됩니다.
	void CloseWnd(class UDefaultWndBase* defaultWnd);
	
	// 각 창들을 생성하는 메서드입니다
	void CreateInventoryWnd();
	void CreateEquipmentWnd();
	void CreateOptionWnd();
	void CreateNewShopWnd(int npcCode);

	//플레이어 컨트롤러를 지정할 때 사용합니다.
	void SetPlayerController(class ACharacterController* playerController);
	
	// 게임모드를 초기화합니다.
	void ResetGameMode();

	// 포커스를 현재 선택된 창으로 바꿉니다.
	void SetFocusWnd(UDefaultWndBase* focusWnd);

	// 포커스 되어있는 창을 지웁니다.
	void CloseFocusWnd();

	// 창의 이전 위치를 저장합니다.
	void SaveWndPos(class UDefaultWndBase* saveWnd);

	//HPBar의 텍스트와 퍼센트를 지정합니다.
	void SetHPBar(int currentHP, int maxHP);

	//상호작용 위젯을 생성/ 삭제합니다.
	void CreateInteractMessage(class ABaseNpc* interactNpcc);
	void DeleteInteractMessage(class ABaseNpc* interactNpcc);

	// 스크린 모드가  바뀔 때 사용됩니다.
	void OnScreenOptionChanged();
	
	// 자식위젯을 생성합니다.
	void AddChildWidget(class UUserWidget* newChildWidget);

	// 다이얼로그를 생성/ 초기화합니다.
	void CreateDialog(class UNpcDialog* newDialog);
	void ClosedDialogWidget();

	// 거래 확인 박스를 생성합니다.
	void CreateTradeCheckBox(int itemCode, int maxItemCount, int itemCost, int& currentMoney);

	//알람 상자를 생성합니다.
	void CreateNoticeBox(FText titleText, FText messageText);

	//모든 창을 닫습니다.
	void CloseAllWnd();

	FORCEINLINE void SetIsTrade(bool newTradeState)
	{ bTrade = newTradeState; }
	FORCEINLINE bool GetIsTrade() const
	{ return bTrade;}

	FORCEINLINE UInventoryWnd* GetInventoryWnd()
	{ return InventoryWnd; }

	FORCEINLINE UEquipmentWnd* GetEquipWnd()
	{
		return EquipWnd;
	}

	FORCEINLINE ACharacterController* GetPlayerController() const
	{
		return PlayerController;
	}

	FORCEINLINE UInteractableMessage* GetInteractableMessage() const
	{
		return InteractableMessage;
	}
	FORCEINLINE int32 GetWndNum()
	{
		return WndArray.Num();
	}
	FORCEINLINE bool GetIsInteracting() const
	{
	 return bInteract;
	}
};
