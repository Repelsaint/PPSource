// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundOptionBar.h"


USoundOptionBar::USoundOptionBar(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USoundOptionBar::NativeConstruct()
{
	// 슬라이더의 값이 변경되었을 때 호출될 메서드를 바인딩합니다.
	Slider_VolumeSize->OnValueChanged.AddDynamic(this, &USoundOptionBar::SliderSizeChange);
	// 체크박스의 값이 변경되었을 때 호출될 메서드를 바인딩합니다.
	CheckBox_MutableBox->OnCheckStateChanged.AddDynamic(this,&USoundOptionBar::OnCheckBoxChanged);
}


void USoundOptionBar::OnCheckBoxChanged(bool bmutable)
{
	//음소거 값이 참이라면
	if (bmutable)
	{
		// 음소거되지 않음값을 거짓으로 설정하고 볼륨사이즈를 0으로 만듭니다.
		bUnMute = false;
		*VolumeSize = 0;
	}
	else 
	{
		//음소거중이지 않음을 참으로 설정하고 볼륨사이즈를 음소거하기 전의 음량으로 변경합니다.
		bUnMute = true;
		*VolumeSize = PrevVolumeSize;
	}
}

void USoundOptionBar::SliderSizeChange(float volumeSize)
{
	// 슬라이더의 값으로 이전 볼륨사이즈를 변경합니다.
	PrevVolumeSize = volumeSize;

	// 실제 볼륨 사이즈는 UnMute의 bool값을 곱해서 음소거중이라면 0이 되게끔 설정합니다.
	*VolumeSize = (PrevVolumeSize * bUnMute);
	
	// 슬라이더값으로 텍스트 사이즈를 지정합니다.
	RefreshSizeText();
}

void USoundOptionBar::RefreshSizeText()
{
	// 슬라이더값은 0~1 사이의 값이므로 0~100사이로 표기되는 값을 표기하기위해 100을 곱합니다.
	int textValue = ((PrevVolumeSize) * 100);
	// 곱해진 값으로 텍스트를 표기합니다.
	Text_Size->SetText(FText::FromString(FString::FromInt(textValue)));
}

void USoundOptionBar::SetMutable(bool mutableState)
{
	// 옵션에서 저장하고있는 값은 UnMutable의 값이므로 체크박스는 거꾸로 적용시켜줍니다.
	if (mutableState)
	{
		// 체크박스를 해제하고 볼륨의 크기를 저장된 볼륨값으로 지정합니다.
		CheckBox_MutableBox->SetCheckedState(ECheckBoxState::Unchecked);
		bUnMute = true;
		*VolumeSize = PrevVolumeSize;

	}
	else
	{
		//체크박스를 체크하고 볼륨사이즈를 0으로 지정합니다.
		CheckBox_MutableBox->SetCheckedState(ECheckBoxState::Checked);
		bUnMute = false;
		*VolumeSize = 0;
	}
}

void USoundOptionBar::SetPrevVolumeSize(float volumeSize)
{
	// 슬라이더 값을 들어온 값으로 변경하고 저장합니다.
	PrevVolumeSize = volumeSize;
	SetSliderSize(PrevVolumeSize);
}
