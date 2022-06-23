// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSUPP.h"
#include "Blueprint/UserWidget.h"	
#include "Components/TextBlock.h"
#include "Components/Slider.h"
#include "Components/CheckBox.h"

#include "SoundOptionBar.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API USoundOptionBar : public UUserWidget
{
	GENERATED_BODY()

	
protected:
	// ���� ����ϰ��ִ� ������ �ɼ� �̸��� ��Ÿ���ϴ�.
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Text_BarName;

	// ���ҰŰ��� ������ ������ �����ߴ� ������ ����� �����ֽ��ϴ�.
	UPROPERTY(meta = (BindWidget))
		USlider* Slider_VolumeSize;
	// ���ҰŰ��Դϴ�.
	UPROPERTY(meta = (BindWidget))
		UCheckBox* CheckBox_MutableBox;
	// ���� �����̴��� ������ ���� ������ ��Ÿ���ϴ�.
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Text_Size;

	// ���� ������ ũ���Դϴ�.
	float* VolumeSize;
	// �����̴� ���� ���� ũ���Դϴ�.
	float PrevVolumeSize;
	// ���Ұ��� ��� 0 �ƴҰ�� 1�� ���ϱ� ���� bool���Դϴ�.
	bool bUnMute;

private:
	//üũ�ڽ��� �ٲ� �� ȣ��˴ϴ�.
	UFUNCTION()
	void OnCheckBoxChanged(bool bmutable);

public:
	USoundOptionBar(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct(); 
	
	// �����̴����� �����Ҷ� ȣ��˴ϴ�.
	UFUNCTION()
	void SliderSizeChange(float volumeSize);
	
	// ������ �ؽ�Ʈ�� ������Ʈ�մϴ�.
	void RefreshSizeText();

	FORCEINLINE UTextBlock* GetBarName()
	{ return Text_BarName; }

	FORCEINLINE void SetBarName(FString barName)
	{ Text_BarName->SetText(FText::FromString(barName)); }

	FORCEINLINE USlider* GetSlider()
	{ return Slider_VolumeSize; }

	FORCEINLINE void SetSliderSize(float value)
	{ Slider_VolumeSize->SetValue(value); }

	FORCEINLINE bool GetMutable()
	{ return bUnMute; }
	void SetMutable(bool mutableState);

	FORCEINLINE UTextBlock* GetSizeText()
	{ return Text_Size; }	
	


	FORCEINLINE void SetVolumeType(float& volumeSize)
	{  VolumeSize = &volumeSize; }
	
	 void SetPrevVolumeSize(float volumeSize);

	 FORCEINLINE float GetPrevVolumeSize() const
	 { return PrevVolumeSize; } 


};
