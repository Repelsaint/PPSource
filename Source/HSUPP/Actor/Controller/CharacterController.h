// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSUPP.h"
#include "GameFramework/PlayerController.h"
#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"

#include "CharacterController.generated.h"

/**
 * 
 */

DECLARE_DELEGATE(FToggleEvent)

UCLASS()
class HSUPP_API ACharacterController : public APlayerController
{
	GENERATED_BODY()

private:

	// �������� �������Ʈ�Դϴ�.
	TSubclassOf<class UMainWidget> BP_MainWidget;
	UPROPERTY(VisibleAnywhere)
		class UMainWidget* MainWidget;

	// �ε� ���� �������Ʈ
	TSubclassOf<class UUserWidget> BP_LoadingWidget;
	class UUserWidget* LoadingWidget;

	//  Ȩ ���� �������Ʈ
	TSubclassOf<class UHomeWidget> BP_HomeWidget;
	class UHomeWidget* HomeWidget;

	// �÷��̾� ĳ���� �Դϴ�.
	APlayerCharacter* PlayerCharacter;

public:
	ACharacterController();

private:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* aPawn) override;

	// ���Ű�� ������ â�� ���� ����� �븮�� �Դϴ�.
	FToggleEvent EquipOpen;

	// �κ��丮Ű�� ������ â�� ���� ����� �븮�� �Դϴ�.
	FToggleEvent InventoryOpen;

	// esc�� �������� ����� �븮���Դϴ�.
	FToggleEvent PressEscButton;



public:
	//ȭ�� XY�� �߰��ϴ� �޼����Դϴ�.
	void AddMouseX(float axis);
	void AddMouseY(float axis);

	//��ȣ�ۿ�� ȣ��Ǵ� �޼���
	void StartInteract();

	// �븮�ڵ� ȣ���ϴ� �޼����Դϴ�.
	FORCEINLINE void EscButtonEvent()
	{
		PressEscButton.ExecuteIfBound();
	}

	FORCEINLINE void OpenEquipWnd()
	{
		EquipOpen.ExecuteIfBound();

	}
	FORCEINLINE void OpenInventoryWnd()
	{
		InventoryOpen.ExecuteIfBound();
	}
};
