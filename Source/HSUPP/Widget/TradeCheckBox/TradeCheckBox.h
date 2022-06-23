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
	// ������ �����Ҽ��ִ� �ؽ�Ʈ�Դϴ�.
	UPROPERTY(meta = (BindWidget))
		class UEditableText* EditableText_NumberText;
	// ������ �����Ҽ��ִ� �����̴��Դϴ�.
	UPROPERTY(meta = (BindWidget))
		class USlider* Slider_NumberSlider;
	// Ȯ�� ��� ��ư�Դϴ�.
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Ok;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Cancel;
	// �Ѻ���� ��Ÿ�� �ؽ�Ʈ�Դϴ�.
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextBlock_TotalCost;

	// üũ Ȯ���� �ؾ��ϴ� �������� �����Դϴ�.
	int MaxCount;
	int ItemCost;
	int TradeCount;
	int ItemCode;
	int ItemMaxCount;

public:
	virtual void NativeConstruct() override;

	// Ʈ���̵� �ڽ��� �ʱ�ȭ�մϴ�.
	void InitTradeBox(int itemCode,int maxItemCount,int itemCost, int& currentMoney);

	// �� �Ǹ� �ݾ��� �����մϴ�.
	void SetTotalCostText(int itemCost);

	//�ؽ�Ʈ���ڰ� �ٲ� ȣ��˴ϴ�.
	UFUNCTION()
	void ChangeTradeText(const FText& newTradeNum);
	// �����̴� ���� �ٲ� ���˴ϴ�.
	UFUNCTION()
	void ChangeTradeSlider(float value);
	// �ŷ��� �����Ҷ� ȣ��˴ϴ�.
	UFUNCTION()
	void TradeStart();
	
};
