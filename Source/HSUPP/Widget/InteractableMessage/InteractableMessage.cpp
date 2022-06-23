// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableMessage.h"

#include "Actor/Character/BaseNpc/BaseNpc.h"
#include "Components/CanvasPanelSlot.h"

void UInteractableMessage::NativeConstruct()
{
	Super::NativeConstruct();
	// ������ ��ġ�� �������ݴϴ�.
	SetPosition();
	
	
}

void UInteractableMessage::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry,InDeltaTime);
	
	//�÷��̾� ��Ʈ�ѷ��� ������ NPC�� �������� �ʴٸ� �������� �ʽ��ϴ�.
	if(TargetNpc == nullptr) return;
	if(PlayerController == nullptr) return;

	// ��ġ�� �����մϴ�.
	SetPosition();
}

void UInteractableMessage::SetDPI(FVector2D newDPI)
{
	//�ػ󵵿� ���� �����ǰ� ������ ���� DPI �����Դϴ�.
	CurrentDPI = newDPI;
}

void UInteractableMessage::SetPosition()
{
	//���� NPC������ ȭ��� ��ġ�� �ҷ��ɴϴ�.
	FVector returnVector;
	FVector2D targetScreenPosition;
	returnVector = TargetNpc->GetActorLocation();
	UGameplayStatics::ProjectWorldToScreen(PlayerController, returnVector, targetScreenPosition);

	// Ÿ�ٽ�ũ�� �������� �ʹ� �ָ� ������ ���̻� �������� �ʽ��ϴ�.
	if(targetScreenPosition.X > 2560.0f || targetScreenPosition.Y > 1440.0f)
	{
		targetScreenPosition = FVector2D(1920.0f, 1080.0f);
	}
	else
	{
		// �о�� ��ġ�� �����մϴ�.
		targetScreenPosition *= CurrentDPI;
	}
	//	������ ������ �޼����� ��ġ�� �����մϴ�.
		Cast<UCanvasPanelSlot>(this->Slot)->SetPosition(targetScreenPosition);
}
