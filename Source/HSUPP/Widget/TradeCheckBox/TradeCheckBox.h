// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSUPP.h"
#include "Widget/DefaultWndBase.h"
#include "TradeCheckBox.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API UTradeCheckBox : public UDefaultWndBase
{
	GENERATED_BODY()
private:
	// 수량을 변경할수있는 텍스트입니다.
	UPROPERTY(meta = (BindWidget))
		class UEditableText* EditableText_NumberText;
	// 수량을 변경할수있는 슬라이더입니다.
	UPROPERTY(meta = (BindWidget))
		class USlider* Slider_NumberSlider;
	// 확인 취소 버튼입니다.
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Ok;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Cancel;
	// 총비용을 나타낼 텍스트입니다.
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextBlock_TotalCost;

	// 체크 확인을 해야하는 아이템의 정보입니다.
	int MaxCount;
	int ItemCost;
	int TradeCount;
	int ItemCode;
	int ItemMaxCount;

public:
	virtual void NativeConstruct() override;

	// 트레이드 박스를 초기화합니다.
	void InitTradeBox(int itemCode,int maxItemCount,int itemCost, int& currentMoney);

	// 총 판매 금액을 설정합니다.
	void SetTotalCostText(int itemCost);

	//텍스트숫자가 바뀔때 호출됩니다.
	UFUNCTION()
	void ChangeTradeText(const FText& newTradeNum);
	// 슬라이더 값이 바뀔때 사용됩니다.
	UFUNCTION()
	void ChangeTradeSlider(float value);
	// 거래를 시작할때 호출됩니다.
	UFUNCTION()
	void TradeStart();
	
};
