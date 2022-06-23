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
	//애니메이션인스턴스를 소지하는 액터입니다.
	class APlayerCharacter* OwnerCharacter;

	class USoundWave* WalkFirstSound;
	class USoundWave* WalkSecondSound;
	
protected:
	// 애니메이션 속도를 결정하는 변수입니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float MoveSpeed;
		// 유저가 공중에 떠있는지를 체크하는 변수입니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsInAir;

		// 플레이어의 점프가 끝났음을 나타냅니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bJumpEnd;

		// 만약 점프 애니메이션이 끝나고서도 땅에 닿지 않았을떄 점프루프 애님을 실행시키기위한 변수입니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bContinueFalling;


public:
	UPlayerAnimInst();
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float dt) override;

public:

	// 점프가 끝날때 호출됩니다. 애니메이션에서 호출됩니다.
	UFUNCTION()
		void AnimNotify_JumpEnd();

	// 점프가 끝나감에도 땅에 닿지않아서 애니메이션이 계속진행될경우 호출됩니다. 애니메이션에서 호출됩니다.
	UFUNCTION()
		void AnimNotify_Falling();

	// 플레이어의 이동을 가능하게합니다. 애니메이션에서 호출됩니다.
	UFUNCTION()
		void AnimNotify_CanMove();

	// 플레이어의 이동을 막습니다. 애니메이션에서 호출됩니다.
	UFUNCTION()
		void AnimNotify_StopMove();

	// 플레이어의 다음 공격을 미리 대기열에 추가할수있도록합니다. 애니메이션에서 호출됩니다.
	UFUNCTION()
		void AnimNotify_CanWaitSkill();

	// 플레이어의 공격 애니메이션이 끝남을 나타냅니다. 애니메이션에서 호출됩니다.
	UFUNCTION()
		void AnimNotify_AnimEnd();
	
	// 플레이어의 공격시 공격 범위를 지정하기위해 호출됩니다. 애니메이션에서 호출됩니다.
	UFUNCTION()
		void AnimNotify_AttackRangeCreate();

	// 플레이어 사망시 리스폰을 위해 호출됩니다. 애니메이션에서 호출됩니다.
	UFUNCTION()
		void AnimNotify_PlayerRespawn();

	// 플레이어가 걸을때 소리를 실행시키기위한 메서드들 입니다.
	UFUNCTION()
		void AnimNotify_WalkFirst();
	UFUNCTION()
		void AnimNotify_WalkSecond();
};
