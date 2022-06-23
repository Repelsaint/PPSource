// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSUPP.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API UPlayerMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

private:
	//소유중인 캐릭터입니다.
	class APlayerCharacter* PlayerCharacter;

	

public:
	UPlayerMovementComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

public:
	// 플레이어가 앞으로 이동할때 호출됩니다.
	void PlayerMoveForward(float axis);

	//플레이어가 옆으로 이동할때 호출됩니다.
	void PlayerMoveRight(float axis);

	// 플레이어가 점프할때 호출됩니다.
	void PlayerJump();

	//플레이어가 달리기를 시작/ 끝낼때 호출됩니다.
	void PlayerRunStart();
	void PlayerRunEnd();



};
