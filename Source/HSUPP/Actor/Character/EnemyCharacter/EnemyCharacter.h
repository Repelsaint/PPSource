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
	// ���� ������ ������ ����
	class UDataTable* DT_EnemyInfo;

	// �� �����ڵ�
	int EnemyCode;

	// ���� �̵��ӵ�
	float EnemySpeed;

	// ���� ����� ����� ����ġ�� ���
	int DropExp;
	int DropGold;

	//���� �⺻ ����
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
	
	// �����ڵ带 �����ϴ� �޼����Դϴ�.
	void SetEnemyCode(int enemyCode);

	// ���� ������ �ʱ�ȭ���ִ� �޼����Դϴ�.
	void InitializeEnemyCharacter();

	// ������ �߻��ϴ� �������� ����� ó���ϴ� �޼����Դϴ�.
	virtual void CharacterDie() override;
	virtual void GetDamaged(int damaged) override;

	// ���� ����� �������� �����ִ� �޼����Դϴ�.
	void EnemyRespawn();

	// ���� ������ �õ��Ҷ� ȣ��Ǵ� �޼����Դϴ�.
	bool TryEnemyAttack();

	// ���� �������Ҷ��� �޼����Դϴ�.
	bool EnemyAttack();

	// ���� �̵��ӵ��� �����ϴ� �޼����Դϴ�.
	void SetEnemyWalkSpeed(float walkSpeed);

	

	// ���� �ӵ��� �������� �޼����Դϴ�.
	FORCEINLINE float GetEnemySpeed() const
	{ return EnemySpeed; }

	// ���� ���ݷ��� �������� �޼����Դϴ�.
	FORCEINLINE int GetEnemyAtk() const
	{ return Atk;}

	
};
