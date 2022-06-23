// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

private:
	//�������� ���͸� ��Ÿ���ϴ�.
	class AEnemyCharacter* OwnerCharacter;

	// �ִϸ��̼� �ӵ� ���� �����ϱ����ؼ� ���Ǵ� �����Դϴ�.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MoveSpeed;

	// ���� ��� ���� ó���Ҷ� ����� bool���Դϴ�.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsAlive;

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float dt) override;

public:
	//���� ���� ���¸� �����մϴ�.
	void ChangeAliveState(bool state);

	// ���� �׾����� ȣ��� �޼����Դϴ�. �ִϸ��̼��ʿ��� ȣ���� �˴ϴ�.
	UFUNCTION()
		void AnimNotify_EnemyDying();


		// ���� ������ �õ��մϴ�. �ִϸ��̼� �ʿ��� ȣ���� �˴ϴ�.
	UFUNCTION()
		void AnimNotify_EnemyTryAttack();

		// ���� �̵��� ����ϴ�. �ִϸ��̼� �ʿ��� ȣ���� �˴ϴ�.
	UFUNCTION()
		void AnimNotify_EnemyStopMove();
		// ���� �̵��� �簳�մϴ�. �ִϸ��̼� �ʿ��� ȣ���� �˴ϴ�.
	UFUNCTION()
		void AnimNotify_EnemyCanMove();
	
};
