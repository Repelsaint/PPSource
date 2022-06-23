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

	// 메인위젯 블루프린트입니다.
	TSubclassOf<class UMainWidget> BP_MainWidget;
	UPROPERTY(VisibleAnywhere)
		class UMainWidget* MainWidget;

	// 로딩 위젯 블루프린트
	TSubclassOf<class UUserWidget> BP_LoadingWidget;
	class UUserWidget* LoadingWidget;

	//  홈 위젯 블루프린트
	TSubclassOf<class UHomeWidget> BP_HomeWidget;
	class UHomeWidget* HomeWidget;

	// 플레이어 캐릭터 입니다.
	APlayerCharacter* PlayerCharacter;

public:
	ACharacterController();

private:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* aPawn) override;

	// 장비키를 눌러서 창을 열때 사용할 대리자 입니다.
	FToggleEvent EquipOpen;

	// 인벤토리키를 눌러서 창을 열때 사용할 대리자 입니다.
	FToggleEvent InventoryOpen;

	// esc를 눌렀을때 사용할 대리자입니다.
	FToggleEvent PressEscButton;



public:
	//화면 XY값 추가하는 메서드입니다.
	void AddMouseX(float axis);
	void AddMouseY(float axis);

	//상호작용시 호출되는 메서드
	void StartInteract();

	// 대리자들 호출하는 메서드입니다.
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
