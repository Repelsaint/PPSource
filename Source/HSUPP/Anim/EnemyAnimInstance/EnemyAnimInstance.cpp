// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "Actor/Character/EnemyCharacter/EnemyCharacter.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// �����ϰ��ִ� ĳ���͸� �����մϴ�.
	OwnerCharacter = Cast<AEnemyCharacter>(TryGetPawnOwner());

	// ������� ���¸� �ʱ�ȭ �մϴ�.
	bIsAlive = true;

}

void UEnemyAnimInstance::NativeUpdateAnimation(float dt)
{
	Super::NativeUpdateAnimation(dt);

	// ���� �������� �ƴ϶�� �������� �ʽ��ϴ�.
	if(!IsValid(OwnerCharacter)) return;

	// �̵��ӵ� ������ ������ ���ʹ̰� �����̴� ������ �������ݴϴ�.
	MoveSpeed = OwnerCharacter->GetVelocity().Size();
}

void UEnemyAnimInstance::ChangeAliveState(bool state)
{	
	//������� ���¸� �����մϴ�.
	bIsAlive = state;
}


void UEnemyAnimInstance::AnimNotify_EnemyDying()
{
	// ���� ������� ���ü��� ���� ������ �ʵ��� �մϴ�.
	OwnerCharacter->GetMesh()->SetVisibility(false);

	// Ÿ�̸Ӹ� �̿��Ͽ� ��� ��Ȱ�ϴ°��� �ƴ� ���� �ð� �Ŀ� ��Ȱ�Ҽ� �ֵ��� �����մϴ�.
	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate;
	timerDelegate.BindUObject(OwnerCharacter ,&AEnemyCharacter::EnemyRespawn);

	GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, 3.0f, false);
}

void UEnemyAnimInstance::AnimNotify_EnemyTryAttack()
{
	// Ư�� �������� ���ʹ̰� ������ �ϵ��� �Ҷ� ����մϴ�.
	OwnerCharacter->EnemyAttack();
}

void UEnemyAnimInstance::AnimNotify_EnemyStopMove()
{
	// ���ʹ̰� �����̸� �ȵɶ� ����� �޼����Դϴ�.
	OwnerCharacter->SetEnemyWalkSpeed(0.0f);
}

void UEnemyAnimInstance::AnimNotify_EnemyCanMove()
{	
	//���ʹ̰� �ٽ� Ȱ���� �Ҽ� �ְ� �Ҷ� ������ ���ǵ� �Դϴ�.
	OwnerCharacter->SetEnemyWalkSpeed(300.0f);
}
