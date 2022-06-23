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
	//소유중인 액터를 나타냅니다.
	class AEnemyCharacter* OwnerCharacter;

	// 애니메이션 속도 등을 제어하기위해서 사용되는 변수입니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MoveSpeed;

	// 적의 사망 등을 처리할때 사용할 bool값입니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsAlive;

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float dt) override;

public:
	//적의 생존 상태를 변경합니다.
	void ChangeAliveState(bool state);

	// 적이 죽었을때 호출될 메서드입니다. 애니메이션쪽에서 호출이 됩니다.
	UFUNCTION()
		void AnimNotify_EnemyDying();


		// 적이 공격을 시도합니다. 애니메이션 쪽에서 호출이 됩니다.
	UFUNCTION()
		void AnimNotify_EnemyTryAttack();

		// 적의 이동을 멈춥니다. 애니메이션 쪽에서 호출이 됩니다.
	UFUNCTION()
		void AnimNotify_EnemyStopMove();
		// 적이 이동을 재개합니다. 애니메이션 쪽에서 호출이 됩니다.
	UFUNCTION()
		void AnimNotify_EnemyCanMove();
	
};
