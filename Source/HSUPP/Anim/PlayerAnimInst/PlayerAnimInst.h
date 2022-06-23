// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInst.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API UPlayerAnimInst : public UAnimInstance
{
	GENERATED_BODY()
		
private:
	//�ִϸ��̼��ν��Ͻ��� �����ϴ� �����Դϴ�.
	class APlayerCharacter* OwnerCharacter;

	class USoundWave* WalkFirstSound;
	class USoundWave* WalkSecondSound;
	
protected:
	// �ִϸ��̼� �ӵ��� �����ϴ� �����Դϴ�.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float MoveSpeed;
		// ������ ���߿� ���ִ����� üũ�ϴ� �����Դϴ�.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsInAir;

		// �÷��̾��� ������ �������� ��Ÿ���ϴ�.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bJumpEnd;

		// ���� ���� �ִϸ��̼��� �������� ���� ���� �ʾ����� �������� �ִ��� �����Ű������ �����Դϴ�.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bContinueFalling;


public:
	UPlayerAnimInst();
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float dt) override;

public:

	// ������ ������ ȣ��˴ϴ�. �ִϸ��̼ǿ��� ȣ��˴ϴ�.
	UFUNCTION()
		void AnimNotify_JumpEnd();

	// ������ ���������� ���� �����ʾƼ� �ִϸ��̼��� �������ɰ�� ȣ��˴ϴ�. �ִϸ��̼ǿ��� ȣ��˴ϴ�.
	UFUNCTION()
		void AnimNotify_Falling();

	// �÷��̾��� �̵��� �����ϰ��մϴ�. �ִϸ��̼ǿ��� ȣ��˴ϴ�.
	UFUNCTION()
		void AnimNotify_CanMove();

	// �÷��̾��� �̵��� �����ϴ�. �ִϸ��̼ǿ��� ȣ��˴ϴ�.
	UFUNCTION()
		void AnimNotify_StopMove();

	// �÷��̾��� ���� ������ �̸� ��⿭�� �߰��Ҽ��ֵ����մϴ�. �ִϸ��̼ǿ��� ȣ��˴ϴ�.
	UFUNCTION()
		void AnimNotify_CanWaitSkill();

	// �÷��̾��� ���� �ִϸ��̼��� ������ ��Ÿ���ϴ�. �ִϸ��̼ǿ��� ȣ��˴ϴ�.
	UFUNCTION()
		void AnimNotify_AnimEnd();
	
	// �÷��̾��� ���ݽ� ���� ������ �����ϱ����� ȣ��˴ϴ�. �ִϸ��̼ǿ��� ȣ��˴ϴ�.
	UFUNCTION()
		void AnimNotify_AttackRangeCreate();

	// �÷��̾� ����� �������� ���� ȣ��˴ϴ�. �ִϸ��̼ǿ��� ȣ��˴ϴ�.
	UFUNCTION()
		void AnimNotify_PlayerRespawn();

	// �÷��̾ ������ �Ҹ��� �����Ű������ �޼���� �Դϴ�.
	UFUNCTION()
		void AnimNotify_WalkFirst();
	UFUNCTION()
		void AnimNotify_WalkSecond();
};
