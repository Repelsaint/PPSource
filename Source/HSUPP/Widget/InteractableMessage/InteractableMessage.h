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
	//��ȣ�ۿ� �޼����� ��ġ�� ���ϱ� ���� NPC�� ��Ʈ�ѷ��Դϴ�.
	class ABaseNpc* TargetNpc;
	APlayerController* PlayerController;
	// ��ġ ������ �����Դϴ�.
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
