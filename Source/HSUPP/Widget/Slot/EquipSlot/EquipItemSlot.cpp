// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipItemSlot.h"
#include "Widget/Slot/InventorySlot/InventorySlot.h"
#include "Widget/EquipmentWnd/EquipmentWnd.h"
#include "Widget/MainWidget.h"
#include "Widget/InventoryWnd/InventoryWnd.h"


void UEquipItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
    Super::NativeOnDragDetected(InGeometry,InMouseEvent,OutOperation);

    // ������ Ÿ���� ��񽽷����� �����մϴ�.
    SlotType = ESlotType::EquipSlot;

}

void UEquipItemSlot::SlotChange()
{
    //�󽽷��� ã������ �κ��丮 �����츦 ���ɴϴ�.
    UInventoryWnd* InvWnd = GetHInstance()->GetMainWidget()->GetInventoryWnd();
    // ������ �÷��̾� ������ ���ɴϴ�.
    FPlayerInfo & PlayerInfo = GetHInstance()->GetPlayerCharacter()->GetPlayerInfo();

    // �������� ù �󽽷��� ã���ϴ�.
    int newSlotIndex = PlayerInfo.GetEmptyInventorySlot();

    // �󽽷��� ���ٸ� ���� ����մϴ�.
    if(newSlotIndex == -1) return;

    // �󽽷��� ������ ���â�� ������ �����ϰ� ���â�� ����ݴϴ�.
    PlayerInfo.InventoryInfo[newSlotIndex] =  PlayerInfo.EquipInfo[this->EquipSlotType];
    GetHInstance()->GetPlayerCharacter()->GetPlayerInfo().EquipInfo[this->EquipSlotType] = FItemInfo();

    //�̹����� �����մϴ�.
    InvWnd->GetInventorySlot(newSlotIndex)->UpdateItemImage(PlayerInfo.InventoryInfo[newSlotIndex].ItemCode, PlayerInfo.InventoryInfo[newSlotIndex].ItemCount);
    this->UpdateItemImage(0);

    // �����ؽ�Ʈ�� �����մϴ�.
    GetHInstance()->GetMainWidget()->GetEquipWnd()->UpdateEquipInfo(this->EquipSlotType, -PlayerInfo.InventoryInfo[newSlotIndex].ItemValue);
}

bool UEquipItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDrop(InGeometry,InDragDropEvent,InOperation);

    // �����巡�׸� �����ߴ� ������ ���ɴϴ�.
    UBaseSlot* OriginSlot = Cast<USlotDragOperation>(InOperation)->OriginSlot;

    // ������ Ÿ���� �κ��丮�����̶�� ������ �õ��մϴ�.
    if (OriginSlot->GetSlotType() == ESlotType::InventorySlot)
    {
        SwapEquipFromInvSlot(Cast<UInventorySlot>(OriginSlot));

        return true;
    }
    // ���â���� ��񽽷��� Ÿ���� ���� �����Ƿ� �ٲ� �� ������ false�� ��ȯ�մϴ�.
    return false;
}

void UEquipItemSlot::SwapEquipFromInvSlot(UInventorySlot* invSlot)
{
    // �κ��丮�� ���â�� ������ �����մϴ�.
    TMap<int,FItemInfo> &InventoryInfo = GetHInstance()->GetPlayerCharacter()->GetPlayerInfo().GetInventoryInfo();
    TMap<EItemType,FItemInfo> &EquipInfo = GetHInstance()->GetPlayerCharacter()->GetPlayerInfo().GetEquipInfo();

    // �� ������ Ÿ�� / �ε����� �̿��Ͽ� ��ȯ�Ϸ��� �������� ������ �о�ɴϴ�.
    FItemInfo OriginInfo = InventoryInfo[invSlot->GetSlotIndex()];
    FItemInfo SwapInfo = EquipInfo[this->GetSlotEquipType()];

    // �ű���� �����۰� ���â������ Ÿ���� ���ٸ� �����մϴ�.
    if (OriginInfo.ItemType == EquipSlotType)
    {
        // ����� ������ �� Ÿ���� ������ �ٲ㼭 �־��ݴϴ�.
        InventoryInfo[invSlot->GetSlotIndex()] = SwapInfo;
        EquipInfo[this->GetSlotEquipType()] = OriginInfo;

        // ������������ ���ݷ��� ��ȭ�� ����մϴ�.
        OriginInfo.ItemValue -= SwapInfo.ItemValue;

        // �������� �̹����� �����մϴ�.
        invSlot->UpdateItemImage(InventoryInfo[invSlot->GetSlotIndex()].ItemCode, InventoryInfo[invSlot->GetSlotIndex()].ItemCount);
        this->UpdateItemImage(EquipInfo[this->GetSlotEquipType()].ItemCode, EquipInfo[this->GetSlotEquipType()].ItemCount);

        // ������������ ������ ������Ʈ ���ݴϴ�.
        GetHInstance()->GetMainWidget()->GetEquipWnd()->UpdateEquipInfo(OriginInfo.ItemType, OriginInfo.ItemValue);
    }


}

