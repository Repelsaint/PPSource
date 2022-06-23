// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMovementComponent.h"
#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"
#include "Actor/Controller/CharacterController.h"

UPlayerMovementComponent::UPlayerMovementComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//�÷��̾� ĳ���͸� �����մϴ�.
	PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	// �ְ� �̵��ӵ��� �����մϴ�.
	MaxWalkSpeed = 500.0f;
	
}

void UPlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	
	// ������ �������� �����մϴ�.
	JumpZVelocity = 1000.0f;

	//�߷°��� �����մϴ�.
	GravityScale = 2.5f;

}

void UPlayerMovementComponent::PlayerMoveForward(float axis)
{
	//���� �̵��Ҽ� ���ٸ� ���������ʽ��ϴ�.
	if (!PlayerCharacter->GetCanMove()) return;

	//�÷��̾��� ȸ������ ����ϴ�.
	FRotator Rotation(0.0f,PlayerCharacter->GetControlRotation().Yaw,0.0f);
	
	// �÷��̾��� ȸ������ ���� �÷��̾��� �չ����� �����ϴ�.
	FVector ForwardVector = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);

	PlayerCharacter->AddMovementInput(ForwardVector, axis);
}

void UPlayerMovementComponent::PlayerMoveRight(float axis)
{
	//���� �̵��Ҽ� ���ٸ� ���������ʽ��ϴ�.
	if (!PlayerCharacter->GetCanMove()) return;

	//�÷��̾��� ȸ������ ����ϴ�.
	FRotator Rotation(0.0f, PlayerCharacter->GetControlRotation().Yaw, 0.0f);

	// �÷��̾��� ȸ������ ���� �÷��̾��� ������ ������ �����ϴ�.
	FVector RightVector = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

	PlayerCharacter->AddMovementInput(RightVector, axis);
}

void UPlayerMovementComponent::PlayerJump()
{
	//ȣ��� ĳ���͸� ������ŵ�ϴ�.
	PlayerCharacter->Jump();
}

void UPlayerMovementComponent::PlayerRunStart()
{
	//�޸��� �����ϸ� �ִ�ӵ��� 1000���� �������ݴϴ�.
	MaxWalkSpeed = 1000.0f;
}

void UPlayerMovementComponent::PlayerRunEnd()
{	
	// �޸��Ⱑ ������ �ִ�ӵ��� �ٽ� �����ݴϴ�.
	MaxWalkSpeed = 500.0f;
}
