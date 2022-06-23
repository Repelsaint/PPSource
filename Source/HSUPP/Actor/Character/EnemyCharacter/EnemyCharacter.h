// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/BaseCharacter.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

private:
	// 적의 정보를 가지는 변수
	class UDataTable* DT_EnemyInfo;

	// 적 고유코드
	int EnemyCode;

	// 적의 이동속도
	float EnemySpeed;

	// 적의 사망시 드랍할 경험치와 골드
	int DropExp;
	int DropGold;

	//적의 기본 정보
	class UBehaviorTree* BehaviorTree;
	class UAnimMontage* AttackAnim;
	class UParticleSystem* HitParticle;
	class USoundWave* AttackSound;
	class USoundWave* HitSound;

public:
	AEnemyCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


protected:
	virtual void BeginPlay() override;
	virtual void UnPossessed() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// 고유코드를 지정하는 메서드입니다.
	void SetEnemyCode(int enemyCode);

	// 적의 정보를 초기화해주는 메서드입니다.
	void InitializeEnemyCharacter();

	// 전투시 발생하는 데미지와 사망을 처리하는 메서드입니다.
	virtual void CharacterDie() override;
	virtual void GetDamaged(int damaged) override;

	// 적의 사망시 리스폰을 시켜주는 메서드입니다.
	void EnemyRespawn();

	// 적이 공격을 시도할때 호출되는 메서드입니다.
	bool TryEnemyAttack();

	// 적이 공격을할때의 메서드입니다.
	bool EnemyAttack();

	// 적의 이동속도를 지정하는 메서드입니다.
	void SetEnemyWalkSpeed(float walkSpeed);

	

	// 적의 속도를 가져오는 메서드입니다.
	FORCEINLINE float GetEnemySpeed() const
	{ return EnemySpeed; }

	// 적의 공격력을 가져오는 메서드입니다.
	FORCEINLINE int GetEnemyAtk() const
	{ return Atk;}

	
};
