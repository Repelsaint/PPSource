// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMovementComponent.h"
#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"
#include "Actor/Controller/CharacterController.h"

UPlayerMovementComponent::UPlayerMovementComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//플레이어 캐릭터를 지정합니다.
	PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	// 최고 이동속도를 지정합니다.
	MaxWalkSpeed = 500.0f;
	
}

void UPlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	
	// 점프시 점프력을 지정합니다.
	JumpZVelocity = 1000.0f;

	//중력값을 지정합니다.
	GravityScale = 2.5f;

}

void UPlayerMovementComponent::PlayerMoveForward(float axis)
{
	//만약 이동할수 없다면 실행하지않습니다.
	if (!PlayerCharacter->GetCanMove()) return;

	//플레이어의 회전값을 얻습니다.
	FRotator Rotation(0.0f,PlayerCharacter->GetControlRotation().Yaw,0.0f);
	
	// 플레이어의 회전값을 토대로 플레이어의 앞방향을 얻어냅니다.
	FVector ForwardVector = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);

	PlayerCharacter->AddMovementInput(ForwardVector, axis);
}

void UPlayerMovementComponent::PlayerMoveRight(float axis)
{
	//만약 이동할수 없다면 실행하지않습니다.
	if (!PlayerCharacter->GetCanMove()) return;

	//플레이어의 회전값을 얻습니다.
	FRotator Rotation(0.0f, PlayerCharacter->GetControlRotation().Yaw, 0.0f);

	// 플레이어의 회전값을 토대로 플레이어의 오른쪽 방향을 얻어냅니다.
	FVector RightVector = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

	PlayerCharacter->AddMovementInput(RightVector, axis);
}

void UPlayerMovementComponent::PlayerJump()
{
	//호출시 캐릭터를 점프시킵니다.
	PlayerCharacter->Jump();
}

void UPlayerMovementComponent::PlayerRunStart()
{
	//달리기 시작하면 최대속도를 1000으로 조정해줍니다.
	MaxWalkSpeed = 1000.0f;
}

void UPlayerMovementComponent::PlayerRunEnd()
{	
	// 달리기가 끝나면 최대속도를 다시 낮춰줍니다.
	MaxWalkSpeed = 500.0f;
}
