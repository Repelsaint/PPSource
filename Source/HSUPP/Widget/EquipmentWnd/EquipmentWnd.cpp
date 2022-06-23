// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentWnd.h"
#include "Widget/Slot/InventorySlot/InventorySlot.h"
#include "Widget/MainWidget.h"
#include "Components/TextBlock.h"


UEquipmentWnd::UEquipmentWnd(const FObjectInitializer& objectinitializer) :Super(objectinitializer)
{
	//기본 창 사이즈를 지정합니다.
	WndSize = FVector2D(400.0f, 700.0f);
}

void UEquipmentWnd::InitializeEquipWnd()
{
	// 인포텍스트를 찾아서 저장합니다.
	CharacterInfoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_CInfo")));
	// 인포텍스트에 띄워줄 정보를 로드합니다.
	APlayerCharacter* playerCharacter = GetHInstance()->GetPlayerCharacter();
	FPlayerInfo& playerInfo = playerCharacter->GetPlayerInfo();

	
	// 로드한 정보를 인포텍스트에 띄워줍니다
	FString charInfo = (TEXT("ATK = ") + FString::FromInt(playerCharacter->GetPlayerAtk()) + TEXT("\nHP = ") + FString::FromInt(playerCharacter->GetPlayerMaxHP()));
	CharacterInfoText->SetText(FText::FromString(charInfo));

	for (auto equipSlots : EquipSlots)
	{
		equipSlots.Value->UpdateItemImage(playerInfo.EquipInfo[equipSlots.Key].ItemCode);
	}
}

void UEquipmentWnd::NativeConstruct()
{
	Super::NativeConstruct();



	SetTitleText(TEXT("Equipment"));
	
	// 찾을 슬롯객체이름들을 저장합니다.
	TArray<FName> equipSlotName =
	{
		FName(TEXT("BP_EquipSlot_Head")),
		FName(TEXT("BP_EquipSlot_Top")),
		FName(TEXT("BP_EquipSlot_Arm")),
		FName(TEXT("BP_EquipSlot_Bottom")),
		FName(TEXT("BP_EquipSlot_Shoes"))
	};

	// 슬롯이름으로 슬롯객체를 찾고 배열에 저장합니다.
	for (FName slotName : equipSlotName)
	{
		UEquipItemSlot* newSlot = Cast<UEquipItemSlot>(GetWidgetFromName(slotName));

		EquipSlots.Add(newSlot->GetSlotEquipType(), newSlot);
	}


	// 장비창을 초기화합니다.
	InitializeEquipWnd();

}

void UEquipmentWnd::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
}


void UEquipmentWnd::UpdateEquipInfo(EItemType itemType, int value)
{
	// 플레이어 캐릭터를 불러옵니다.
	APlayerCharacter* playerCharacter= GetHInstance()->GetPlayerCharacter();

	if (itemType == EItemType::Arm || itemType == EItemType::Head)
	{
	// 장착한 부위가 팔이나 머리라면 공격력을 상승시킵니다.
		playerCharacter->AddPlayerAtk(value);
	}
	else
	{
	// 그렇지 않다면 체력을 올려줍니다.
		playerCharacter->AddPlayerMaxHP(value);
		MainWidget->SetHPBar(playerCharacter->GetPlayerCurrentHP(), playerCharacter->GetPlayerMaxHP());
	}

	// 변경된 값을 인포텍스트에 적용해줍니다.
	FString charInfo(TEXT("ATK = ") + FString::FromInt(playerCharacter->GetPlayerAtk()) + TEXT("\nHP = ") + FString::FromInt(playerCharacter->GetPlayerMaxHP()));
	CharacterInfoText->SetText(FText::FromString(charInfo));
}
