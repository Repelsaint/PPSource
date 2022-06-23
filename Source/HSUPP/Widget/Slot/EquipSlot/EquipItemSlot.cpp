// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipItemSlot.h"
#include "Widget/Slot/InventorySlot/InventorySlot.h"
#include "Widget/EquipmentWnd/EquipmentWnd.h"
#include "Widget/MainWidget.h"
#include "Widget/InventoryWnd/InventoryWnd.h"


void UEquipItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
    Super::NativeOnDragDetected(InGeometry,InMouseEvent,OutOperation);

    // 슬롯의 타입을 장비슬롯으로 지정합니다.
    SlotType = ESlotType::EquipSlot;

}

void UEquipItemSlot::SlotChange()
{
    //빈슬롯을 찾기위해 인벤토리 윈도우를 얻어옵니다.
    UInventoryWnd* InvWnd = GetHInstance()->GetMainWidget()->GetInventoryWnd();
    // 갱신할 플레이어 정보를 얻어옵니다.
    FPlayerInfo & PlayerInfo = GetHInstance()->GetPlayerCharacter()->GetPlayerInfo();

    // 윈도우의 첫 빈슬롯을 찾습니다.
    int newSlotIndex = PlayerInfo.GetEmptyInventorySlot();

    // 빈슬롯이 없다면 실행 취소합니다.
    if(newSlotIndex == -1) return;

    // 빈슬롯의 정보를 장비창의 정보로 변경하고 장비창을 비워줍니다.
    PlayerInfo.InventoryInfo[newSlotIndex] =  PlayerInfo.EquipInfo[this->EquipSlotType];
    GetHInstance()->GetPlayerCharacter()->GetPlayerInfo().EquipInfo[this->EquipSlotType] = FItemInfo();

    //이미지를 갱신합니다.
    InvWnd->GetInventorySlot(newSlotIndex)->UpdateItemImage(PlayerInfo.InventoryInfo[newSlotIndex].ItemCode, PlayerInfo.InventoryInfo[newSlotIndex].ItemCount);
    this->UpdateItemImage(0);

    // 인포텍스트를 갱신합니다.
    GetHInstance()->GetMainWidget()->GetEquipWnd()->UpdateEquipInfo(this->EquipSlotType, -PlayerInfo.InventoryInfo[newSlotIndex].ItemValue);
}

bool UEquipItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDrop(InGeometry,InDragDropEvent,InOperation);

    // 원래드래그를 시작했던 슬롯을 얻어옵니다.
    UBaseSlot* OriginSlot = Cast<USlotDragOperation>(InOperation)->OriginSlot;

    // 슬롯의 타입이 인벤토리슬롯이라면 변경을 시도합니다.
    if (OriginSlot->GetSlotType() == ESlotType::InventorySlot)
    {
        SwapEquipFromInvSlot(Cast<UInventorySlot>(OriginSlot));

        return true;
    }
    // 장비창에서 장비슬롯은 타입이 맞지 않으므로 바뀔 수 없으니 false를 반환합니다.
    return false;
}

void UEquipItemSlot::SwapEquipFromInvSlot(UInventorySlot* invSlot)
{
    // 인벤토리와 장비창의 정보를 참조합니다.
    TMap<int,FItemInfo> &InventoryInfo = GetHInstance()->GetPlayerCharacter()->GetPlayerInfo().GetInventoryInfo();
    TMap<EItemType,FItemInfo> &EquipInfo = GetHInstance()->GetPlayerCharacter()->GetPlayerInfo().GetEquipInfo();

    // 각 슬롯의 타입 / 인덱스를 이용하여 변환하려는 아이템의 정보를 읽어옵니다.
    FItemInfo OriginInfo = InventoryInfo[invSlot->GetSlotIndex()];
    FItemInfo SwapInfo = EquipInfo[this->GetSlotEquipType()];

    // 옮기려는 아이템과 장비창슬롯의 타입이 같다면 스왑합니다.
    if (OriginInfo.ItemType == EquipSlotType)
    {
        // 복사된 정보를 각 타입의 인포에 바꿔서 넣어줍니다.
        InventoryInfo[invSlot->GetSlotIndex()] = SwapInfo;
        EquipInfo[this->GetSlotEquipType()] = OriginInfo;

        // 스왑으로인한 공격력의 변화를 계산합니다.
        OriginInfo.ItemValue -= SwapInfo.ItemValue;

        // 아이템의 이미지를 갱신합니다.
        invSlot->UpdateItemImage(InventoryInfo[invSlot->GetSlotIndex()].ItemCode, InventoryInfo[invSlot->GetSlotIndex()].ItemCount);
        this->UpdateItemImage(EquipInfo[this->GetSlotEquipType()].ItemCode, EquipInfo[this->GetSlotEquipType()].ItemCount);

        // 스왑으로인한 정보를 업데이트 해줍니다.
        GetHInstance()->GetMainWidget()->GetEquipWnd()->UpdateEquipInfo(OriginInfo.ItemType, OriginInfo.ItemValue);
    }


}

