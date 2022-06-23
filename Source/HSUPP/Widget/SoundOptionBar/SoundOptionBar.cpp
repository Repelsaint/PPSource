// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundOptionBar.h"


USoundOptionBar::USoundOptionBar(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USoundOptionBar::NativeConstruct()
{
	// �����̴��� ���� ����Ǿ��� �� ȣ��� �޼��带 ���ε��մϴ�.
	Slider_VolumeSize->OnValueChanged.AddDynamic(this, &USoundOptionBar::SliderSizeChange);
	// üũ�ڽ��� ���� ����Ǿ��� �� ȣ��� �޼��带 ���ε��մϴ�.
	CheckBox_MutableBox->OnCheckStateChanged.AddDynamic(this,&USoundOptionBar::OnCheckBoxChanged);
}


void USoundOptionBar::OnCheckBoxChanged(bool bmutable)
{
	//���Ұ� ���� ���̶��
	if (bmutable)
	{
		// ���Ұŵ��� �������� �������� �����ϰ� ��������� 0���� ����ϴ�.
		bUnMute = false;
		*VolumeSize = 0;
	}
	else 
	{
		//���Ұ������� ������ ������ �����ϰ� ��������� ���Ұ��ϱ� ���� �������� �����մϴ�.
		bUnMute = true;
		*VolumeSize = PrevVolumeSize;
	}
}

void USoundOptionBar::SliderSizeChange(float volumeSize)
{
	// �����̴��� ������ ���� ��������� �����մϴ�.
	PrevVolumeSize = volumeSize;

	// ���� ���� ������� UnMute�� bool���� ���ؼ� ���Ұ����̶�� 0�� �ǰԲ� �����մϴ�.
	*VolumeSize = (PrevVolumeSize * bUnMute);
	
	// �����̴������� �ؽ�Ʈ ����� �����մϴ�.
	RefreshSizeText();
}

void USoundOptionBar::RefreshSizeText()
{
	// �����̴����� 0~1 ������ ���̹Ƿ� 0~100���̷� ǥ��Ǵ� ���� ǥ���ϱ����� 100�� ���մϴ�.
	int textValue = ((PrevVolumeSize) * 100);
	// ������ ������ �ؽ�Ʈ�� ǥ���մϴ�.
	Text_Size->SetText(FText::FromString(FString::FromInt(textValue)));
}

void USoundOptionBar::SetMutable(bool mutableState)
{
	// �ɼǿ��� �����ϰ��ִ� ���� UnMutable�� ���̹Ƿ� üũ�ڽ��� �Ųٷ� ��������ݴϴ�.
	if (mutableState)
	{
		// üũ�ڽ��� �����ϰ� ������ ũ�⸦ ����� ���������� �����մϴ�.
		CheckBox_MutableBox->SetCheckedState(ECheckBoxState::Unchecked);
		bUnMute = true;
		*VolumeSize = PrevVolumeSize;

	}
	else
	{
		//üũ�ڽ��� üũ�ϰ� ��������� 0���� �����մϴ�.
		CheckBox_MutableBox->SetCheckedState(ECheckBoxState::Checked);
		bUnMute = false;
		*VolumeSize = 0;
	}
}

void USoundOptionBar::SetPrevVolumeSize(float volumeSize)
{
	// �����̴� ���� ���� ������ �����ϰ� �����մϴ�.
	PrevVolumeSize = volumeSize;
	SetSliderSize(PrevVolumeSize);
}
