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
	//�������� ĳ�����Դϴ�.
	class APlayerCharacter* PlayerCharacter;

	

public:
	UPlayerMovementComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

public:
	// �÷��̾ ������ �̵��Ҷ� ȣ��˴ϴ�.
	void PlayerMoveForward(float axis);

	//�÷��̾ ������ �̵��Ҷ� ȣ��˴ϴ�.
	void PlayerMoveRight(float axis);

	// �÷��̾ �����Ҷ� ȣ��˴ϴ�.
	void PlayerJump();

	//�÷��̾ �޸��⸦ ����/ ������ ȣ��˴ϴ�.
	void PlayerRunStart();
	void PlayerRunEnd();



};
