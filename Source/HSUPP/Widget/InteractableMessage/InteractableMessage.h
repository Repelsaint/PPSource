// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSUPP.h"

#include "Actor/Controller/CharacterController.h"

#include "Blueprint/UserWidget.h"

#include "InteractableMessage.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API UInteractableMessage : public UUserWidget
{
	GENERATED_BODY()

private:
	//상호작용 메세지의 위치를 정하기 위한 NPC와 컨트롤러입니다.
	class ABaseNpc* TargetNpc;
	APlayerController* PlayerController;
	// 위치 보정용 변수입니다.
	FVector2D CurrentDPI;

	
public:

	virtual void NativeConstruct() override;  
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	FORCEINLINE void SetPlayerController(APlayerController* playerController)
	{ PlayerController = playerController; }
	
	FORCEINLINE void SetTargetNpc(ABaseNpc* target)
	{ TargetNpc = target;}

	void SetDPI(FVector2D newDPI);
	
	void SetPosition();
	
};
