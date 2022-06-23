// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "Actor/Character/EnemyCharacter/EnemyCharacter.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// 소유하고있는 캐릭터를 지정합니다.
	OwnerCharacter = Cast<AEnemyCharacter>(TryGetPawnOwner());

	// 살아있음 상태를 초기화 합니다.
	bIsAlive = true;

}

void UEnemyAnimInstance::NativeUpdateAnimation(float dt)
{
	Super::NativeUpdateAnimation(dt);

	// 만약 소유중이 아니라면 실행하지 않습니다.
	if(!IsValid(OwnerCharacter)) return;

	// 이동속도 변수를 실제로 에너미가 움직이는 값으로 지정해줍니다.
	MoveSpeed = OwnerCharacter->GetVelocity().Size();
}

void UEnemyAnimInstance::ChangeAliveState(bool state)
{	
	//살아있음 상태를 지정합니다.
	bIsAlive = state;
}


void UEnemyAnimInstance::AnimNotify_EnemyDying()
{
	// 적이 죽을경우 가시성을 꺼서 보이지 않도록 합니다.
	OwnerCharacter->GetMesh()->SetVisibility(false);

	// 타이머를 이용하여 즉시 부활하는것이 아닌 일정 시간 후에 부활할수 있도록 지정합니다.
	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate;
	timerDelegate.BindUObject(OwnerCharacter ,&AEnemyCharacter::EnemyRespawn);

	GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, 3.0f, false);
}

void UEnemyAnimInstance::AnimNotify_EnemyTryAttack()
{
	// 특정 시점에서 에너미가 공격을 하도록 할때 사용합니다.
	OwnerCharacter->EnemyAttack();
}

void UEnemyAnimInstance::AnimNotify_EnemyStopMove()
{
	// 에너미가 움직이면 안될때 사용할 메서드입니다.
	OwnerCharacter->SetEnemyWalkSpeed(0.0f);
}

void UEnemyAnimInstance::AnimNotify_EnemyCanMove()
{	
	//에너미가 다시 활동을 할수 있게 할때 지정할 스피드 입니다.
	OwnerCharacter->SetEnemyWalkSpeed(300.0f);
}
