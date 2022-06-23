// Fill out your copyright notice in the Description page of Project Settings.


#include "TradeCheckBox.h"

#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"

#include "Widget/MainWidget.h"
#include "Widget/InventoryWnd/InventoryWnd.h"

#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"

#include "Struct/PlayerInfo.h"

void UTradeCheckBox::NativeConstruct()
{
	Super::NativeConstruct();
	// 에디터박스와 슬라이더 값이 변경될 때 호출될 메서드를 바인딩합니다.
	EditableText_NumberText->OnTextChanged.AddDynamic(this, &UTradeCheckBox::ChangeTradeText);
	Slider_NumberSlider->OnValueChanged.AddDynamic(this, &UTradeCheckBox::ChangeTradeSlider);
	// 취소 / 확인 버튼을 눌렀을 때 호출될 메서드를 바인딩합니다.
	Button_Cancel->OnClicked.AddDynamic(this, &UTradeCheckBox::CloseThisWnd);
	Button_Ok->OnClicked.AddDynamic(this, &UTradeCheckBox::TradeStart);

	// 체크박스가 켜졌을 때 첫 포커싱을 에디터박스가 갖도록 합니다.
	EditableText_NumberText->SetKeyboardFocus();
	
}

void UTradeCheckBox::InitTradeBox(int itemCode,int maxItemCount,int itemCost, int& currentMoney)
{
	//아이템 코스트가 없다면 무료 아이템이므로 최대갯수를 99개로 지정합니다.
	if(itemCost == 0)
		MaxCount = 99;
	//	가격이 존재 한다면 현재 최대로 구매할수 있는 갯수를 구합니다.
	else 
		MaxCount = currentMoney/itemCost;

	// 구매로 한번에 채울수 있는 칸의 수는 10칸까지로 제한하므로 아이템의 맥스카운트의 10배이상에 해당하는
	// 물건을 구매할 수있다면 맥스카운트의 10배까지로 수량을 제한합니다.
	if(MaxCount > (maxItemCount*10))
		MaxCount = maxItemCount * 10;
	
	// 구매하려는 아이템의 정보를 저장합니다.
	ItemCost = itemCost;
	ItemCode = itemCode;
	ItemMaxCount = maxItemCount;

	// 최대 구매 가능 숫자로 슬라이더 최댓값을 변경합니다.
	Slider_NumberSlider->SetMaxValue(MaxCount);

	// 총 구매 필요 금액을 설정합니다.
	SetTotalCostText(itemCost);
}

void UTradeCheckBox::SetTotalCostText(int itemCost)
{
	// 현재 총 필요 금액을 FText 형식으로 변환한뒤 저장합니다.
	FText costText = FText::Format(FText::FromString(TEXT("총 금액 : {0}")),itemCost);
	TextBlock_TotalCost->SetText(costText);
}

void UTradeCheckBox::ChangeTradeText(const FText& newTradeNum)
{
	// 에디터 박스에 제대로 숫자가 입력되었는지 체크합니다.
	if (newTradeNum.IsNumeric())
	{
		//트레이드 숫자를 int로 형변환 합니다.
		TradeCount = FCString::Atoi(*newTradeNum.ToString());
		if (TradeCount > MaxCount)
		{
			//입력된 값이 맥스카운트보다 높다면 입력값을 맥스카운트로 변경합니다.
			//텍스트값을 최대치로 바꿔줍니다.
			TradeCount = MaxCount;
			EditableText_NumberText->SetText(FText::FromString(FString::FromInt(MaxCount)));
		}
		
		// 전체 금액을 지정하고 슬라이더 값을 변경해줍니다.
		SetTotalCostText(ItemCost * TradeCount);
		Slider_NumberSlider->SetValue(TradeCount);
	}
	else
	{
		// 숫자이외의 값이 들어왔다면 이전의 트레이드 값으로 유지시킵니다.
		EditableText_NumberText->SetText(FText::FromString(FString::FromInt(TradeCount)));
	}

}


void UTradeCheckBox::ChangeTradeSlider(float value)
{
	//슬라이더 값으로 카운트를 변경합니다.
	TradeCount = value;
	// 에디터박스의 숫자를 슬라이더의 값으로 변경해줍니다.
	EditableText_NumberText->SetText(FText::FromString(FString::FromInt(TradeCount)));

	// 바뀐 금액으로 텍스트를 바꿔줍니다.
	SetTotalCostText(ItemCost * value);
}

void UTradeCheckBox::TradeStart()
{
	//플레이어의 정보를 불러들입니다.
	FPlayerInfo& playerInfo = GetHInstance()->GetPlayerCharacter()->GetPlayerInfo();

	// 전체 금액을 책정합니다.
	int totalCost = TradeCount * ItemCost;

	// 아이템 맥스카운트가 1 이상인 경우(ex포션)
	if(ItemMaxCount > 1)
	{
		// 같은 종류의 아이템이 저장된 칸의 인덱스를 전부 받아옵니다.
		TArray<int> sameSlots = playerInfo.GetSameItemSlot(ItemCode);

		// 같은 아이템이 저장된 칸에 모두 들어가는지 체크할 변수 입니다.
		int totalItemCount = TradeCount;

		// 구매후에  가지게 되는 아이템의 수량의 총량을 계산합니다.
		for (int i = 0; i < sameSlots.Num(); ++i)
		{
			totalItemCount += playerInfo.InventoryInfo[sameSlots[i]].ItemCount;
		}

		// 만약 구매후 총량이 현재 저장된 아이템이 저장된 칸의 최대 저장량보다 많다면
		// 넘치는 양을 빈칸에 저장해줍니다.
		if (totalItemCount > (ItemMaxCount * sameSlots.Num()))
		{
			// 빈칸에 들어가야하는 아이템의 수를 따로 저장합니다.
			int emptySlotItemNum = (totalItemCount - (ItemMaxCount * sameSlots.Num()));
			TradeCount -= emptySlotItemNum;

			// 인벤토리의 빈칸의 인덱스배열입니다.
			TArray<int> emptySlotNum;

			// 빈칸이 충분한지 체크합니다.
			for (int i = 0; i < (emptySlotItemNum / ItemMaxCount)+1; ++i)
			{
				// 빈칸을 가져옵니다.
				int newEmptyNum = playerInfo.GetEmptyInventorySlot();

				if (newEmptyNum == -1) 
				{
					// 빈칸이 부족하므로 에러를 띄웁니다.
					FText titleText = FText::FromString(TEXT("Error"));
					FText messageText = FText::FromString(TEXT("빈 칸이 부족합니다."));
					GetHInstance()->GetMainWidget()->CreateNoticeBox(titleText, messageText);

					// 빈칸을 확인하기위해 변경해뒀던 슬롯들을 다시 0으로 변경해줍니다.
					for(int i = 0; i < emptySlotNum.Num(); ++i)
					{
						playerInfo.InventoryInfo[newEmptyNum].ItemCode = 0;
					}
					// 그후 이 창을 닫습니다.
					CloseThisWnd();
					return;
				}
				else
				{
					// 아이템이 들어갈 자리임을 표기하기 위해 코드를 -1로 임시변경합니다.
					playerInfo.InventoryInfo[newEmptyNum].ItemCode = -1;
					emptySlotNum.Add(newEmptyNum);
				}
			}
			
			// 빈칸이 충분하다면 채워줍니다.
			for (int i = 0; i < emptySlotNum.Num(); ++i)
			{
				// 아까 배열에 담아 두었던 칸에 아이템 코드를 적용합니다.
				playerInfo.InventoryInfo[emptySlotNum[i]].ItemCode = ItemCode;

				// 남아있는 아이템의 수가 현재 칸에 들어갈수있는 총량 보다 크다면
				if (emptySlotItemNum > ItemMaxCount)
				{
					// 아이템을 총량만큼 채운뒤 넣어야 하는 양을 줄여줍니다.
					playerInfo.InventoryInfo[emptySlotNum[i]].ItemCount = ItemMaxCount;
					emptySlotItemNum -= ItemMaxCount;
				}
				// 그렇지 않다면 남은 수량을 빈칸에 채워넣어줍니다.
				else
					playerInfo.InventoryInfo[emptySlotNum[i]].ItemCount = emptySlotItemNum;
			}
		}
		
		// 빈 칸에 들어가야할 아이템의 처리가 모두 끝난후
		// 같은 아이템이 저장된 칸에 구매한 아이템을 채워 넣습니다.
		for (int i = 0; i < sameSlots.Num(); ++i)
		{
			// 현재가지고있는 수량과 남은 아이템 수량을 전부 더합니다.
			int itemCount = playerInfo.InventoryInfo[sameSlots[i]].ItemCount += TradeCount;
			
			// 더한값이 칸 총량보다 많다면
			if (itemCount > ItemMaxCount)
			{
				// 총량만큼만 소지하고 남은 수량은 다음 칸으로 넘깁니다.
				playerInfo.InventoryInfo[sameSlots[i]].ItemCount = ItemMaxCount;
				TradeCount = itemCount - ItemMaxCount;
			}
			else
			{
				// 만약 넘치지 않는다면 남은 수량을 0으로 지정하고 칸의 수량을 변경해줍니다.
				playerInfo.InventoryInfo[sameSlots[i]].ItemCount = itemCount;
				TradeCount = 0;
				break;
			}
		}

	}
	// 장비 등 칸당 최대 저장량이 1인 아이템의 처리입니다.
	else
	{
		//빈칸들의 배열입니다.
		TArray<int> emptySlotNum;
		// 아이템이 들어갈 빈칸이 충분한지 체크합니다. 
		for (int i = 0; i < TradeCount; ++i)
		{
		 // 빈칸이 있는지 체크합니다.
			int newemptyNum = playerInfo.GetEmptyInventorySlot();
			if(newemptyNum == -1)
			{
				// 빈칸이 부족하면 부족하다는 에러메세지를 띄웁니다.
				FText titleText = FText::FromString(TEXT("Error"));
				FText messageText = FText::FromString(TEXT("빈 칸이 부족합니다."));
				GetHInstance()->GetMainWidget()->CreateNoticeBox(titleText, messageText);

				// 빈칸확인을 위해 코드를 바꿔두었던 부분을 원상복귀합니다.
				for(int i = 0; i < emptySlotNum.Num(); ++i)
				{
					playerInfo.InventoryInfo[newemptyNum].ItemCode = 0;
				}
				// 그 후 창을 닫습니다.
				CloseThisWnd();
				return;
			}

			// 이미 검색한 빈칸임을 찾기위해 배열에 저장하고 아이템 코드를 -1로 임시변경해줍니다..
			emptySlotNum.Add(newemptyNum);
			playerInfo.InventoryInfo[newemptyNum].ItemCode = -1;
		}

		// 빈칸이 충분하다면
		for (int i = 0; i < TradeCount; ++i)
		{	
			// 인벤토리 슬롯 칸에 코드랑 수량을 저장합니다.
			playerInfo.InventoryInfo[emptySlotNum[i]].ItemCode = ItemCode;
			playerInfo.InventoryInfo[emptySlotNum[i]].ItemCount = 1;
		}

	}
	
	// 구매가 종료된 후 금액지불과 이미지를 업데이트합니다.
	playerInfo.Money -= totalCost;
	GetHInstance()->GetMainWidget()->GetInventoryWnd()->UpdateAllInventorySlot();

	// 창을 닫습니다.
	CloseThisWnd();
}
