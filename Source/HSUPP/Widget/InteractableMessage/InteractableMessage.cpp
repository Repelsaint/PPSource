// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableMessage.h"

#include "Actor/Character/BaseNpc/BaseNpc.h"
#include "Components/CanvasPanelSlot.h"

void UInteractableMessage::NativeConstruct()
{
	Super::NativeConstruct();
	// 생성시 위치를 조정해줍니다.
	SetPosition();
	
	
}

void UInteractableMessage::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry,InDeltaTime);
	
	//플레이어 컨트롤러나 생성될 NPC가 존재하지 않다면 생성하지 않습니다.
	if(TargetNpc == nullptr) return;
	if(PlayerController == nullptr) return;

	// 위치를 지정합니다.
	SetPosition();
}

void UInteractableMessage::SetDPI(FVector2D newDPI)
{
	//해상도에 따른 포지션값 보정을 위한 DPI 조정입니다.
	CurrentDPI = newDPI;
}

void UInteractableMessage::SetPosition()
{
	//현재 NPC액터의 화면상 위치를 불러옵니다.
	FVector returnVector;
	FVector2D targetScreenPosition;
	returnVector = TargetNpc->GetActorLocation();
	UGameplayStatics::ProjectWorldToScreen(PlayerController, returnVector, targetScreenPosition);

	// 타겟스크린 포지션이 너무 멀리 나가면 더이상 생성하지 않습니다.
	if(targetScreenPosition.X > 2560.0f || targetScreenPosition.Y > 1440.0f)
	{
		targetScreenPosition = FVector2D(1920.0f, 1080.0f);
	}
	else
	{
		// 읽어온 위치를 보정합니다.
		targetScreenPosition *= CurrentDPI;
	}
	//	보정된 값으로 메세지의 위치를 조정합니다.
		Cast<UCanvasPanelSlot>(this->Slot)->SetPosition(targetScreenPosition);
}
