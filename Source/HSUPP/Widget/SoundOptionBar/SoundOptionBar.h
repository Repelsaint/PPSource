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
	// 현재 담당하고있는 사운드의 옵션 이름을 나타냅니다.
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Text_BarName;

	// 음소거값과 별개로 이전에 지정했던 볼륨의 사이즈를 갖고있습니다.
	UPROPERTY(meta = (BindWidget))
		USlider* Slider_VolumeSize;
	// 음소거값입니다.
	UPROPERTY(meta = (BindWidget))
		UCheckBox* CheckBox_MutableBox;
	// 현재 슬라이더로 지정된 값이 얼마인지 나타냅니다.
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Text_Size;

	// 실제 볼륨의 크기입니다.
	float* VolumeSize;
	// 슬라이더 상의 볼륨 크기입니다.
	float PrevVolumeSize;
	// 음소거일 경우 0 아닐경우 1을 곱하기 위한 bool값입니다.
	bool bUnMute;

private:
	//체크박스가 바뀔 때 호출됩니다.
	UFUNCTION()
	void OnCheckBoxChanged(bool bmutable);

public:
	USoundOptionBar(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct(); 
	
	// 슬라이더값을 조정할때 호출됩니다.
	UFUNCTION()
	void SliderSizeChange(float volumeSize);
	
	// 사이즈 텍스트를 업데이트합니다.
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
