// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSUPP.h"
#include "Actor/Character/BaseCharacter.h"

#include "Component/ZoomableSpringArm/ZoomableSpringArm.h"
#include "Component/PlayerMovementComponent/PlayerMovementComponent.h"
#include "Component/PlayerAttack/PlayerAttack.h"

#include "Struct/PlayerInfo.h"

#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()


private:
	//ĳ������ �Ž��� ���������� ����
	class USkeletalMesh* CharacterMesh;
	
	// ī�޶��� �Ÿ��� �����ϴ� ������Ʈ
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UZoomableSpringArm* SpringArm;

	// ī�޶� ������Ʈ
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	// ���� ������ ���������� ����
	UPROPERTY()
	FPlayerInfo PlayerInfo;

	// �÷��̾� �̵� ������Ʈ
	UPlayerMovementComponent* PlayerMoveComponent;

	// �÷��̾� ���� ������Ʈ
	UPlayerAttack* AttackComponent;

	// �÷��̾� �ִϸ��̼� �������Ʈ Ŭ���� 
	TSubclassOf<class UPlayerAnimInst> PlayerAnimClass;

	// ��ȣ�ۿ밡���� ���͸� ������ �迭
	TArray<class UInteractableAreaComponent*> InteractableActors;

	// ������ ���¸� ��Ÿ���� bool��
	bool bCanMove;
	bool bAlive;

private:
	APlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//������ �õ��ϴ� �޼���
	void TryAttack();


protected:
	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ������ �޾������� ó���� �ϴ� �޼����Դϴ�.
	virtual void GetDamaged(int damaged) override;

	// ĳ���Ͱ� �׾������� ó���� �ϴ� �޼����Դϴ�.
	virtual void CharacterDie() override;

	// ĳ������ ������ �ҷ����� �޼����Դϴ�.
	void LoadPlayerInfo();

	// �÷��̾��� ����� �������� �����ִ� �޼����Դϴ�.
	void PlayerRespawn();

	// ��ȣ�ۿ� ������ ������ �迭�� ���͸� �߰����ִ� �޼����Դϴ�.
	void AddInteractableActor(class UInteractableAreaComponent* newActor);

	// ��ȣ�ۿ� �������� ������������ ���͸� �迭���� ���ִ� �޼����Դϴ�.
	void RemoveInteractableActor(class UInteractableAreaComponent* deleteActor);

	// �÷��̾��� �������¸� �����ϴ� �޼����Դϴ�.
	FORCEINLINE void SetAlive(bool aliveState)
	{ bAlive = aliveState;}
	
	// �÷��̾��� �������¸� Ȯ���Ҷ� ���Ǵ� �޼����Դϴ�.
	FORCEINLINE bool IsAlive() const
	{ return bAlive; }

	// �÷��̾��� ����������Ʈ�� �ҷ����� ���Ǵ� �ż����Դϴ�.
	FORCEINLINE UPlayerAttack* GetAttackComponent() const
	{
		return AttackComponent;
	}
	// �÷��̾��� ��������������Ʈ�� �ҷ����� ���Ǵ� �ż����Դϴ�.
	FORCEINLINE UZoomableSpringArm* GetSpringArm() const
	{
		return SpringArm;
	}
	// �÷��̾��� ����������Ʈ�� �ҷ����� ���Ǵ� �ż����Դϴ�.
	FORCEINLINE UPlayerMovementComponent* GetPlayerMovementComponent() const
	{
		return PlayerMoveComponent;
	}

	// ���� �÷��̾ �����ϼ��ִ��� Ȯ���ϴ� �޼����Դϴ�.
	FORCEINLINE bool GetCanMove() const
	{
		return bCanMove;
	}

	// �÷��̾��� �̵� ���ɻ��¸� �����Ҷ� ����ϴ� �޼����Դϴ�.
	FORCEINLINE void SetMovable(bool moveType)
	{
		bCanMove = moveType;
	}

	// �÷��̾��� ������ �����Ҷ� ����ϴ� �޼����Դϴ�.
	FORCEINLINE void SetPlayerInfo(FPlayerInfo playerInfo)
	{
		PlayerInfo = playerInfo;
	}

	// �÷��̾��� ������ �о���϶� ����ϴ� �޼����Դϴ�.
	// ���縦 �̿��ϸ� ���ʿ��� ���� ���� ����ǹǷ� ������ �̿��Ͽ����ϴ�.
	FORCEINLINE FPlayerInfo &GetPlayerInfo()
	{
		return PlayerInfo;
	}

	// �÷��̾��� ����ü���� ������ �Ͼ���� ���Ǵ� �޼����Դϴ�.
	FORCEINLINE void AddPlayerCurrentHP(int currentHP) 
	{
		PlayerInfo.CurrentHP += currentHP;
	}

	// �÷��̾��� �ִ�ü���� ������ �Ͼ���� ���Ǵ� �޼����Դϴ�.
	FORCEINLINE void AddPlayerMaxHP(int maxHP)
	{
		PlayerInfo.MaxHP += maxHP;
	}

	// �÷��̾��� ���ݷ��� �߰��Ҷ� ���Ǵ� �޼����Դϴ�.
	FORCEINLINE void AddPlayerAtk(int atk)
	{
		PlayerInfo.Atk += atk;
	}

	// �÷��̾��� ���� ü���� �ҷ��ɴϴ�.
	FORCEINLINE int GetPlayerCurrentHP()
	{
		return PlayerInfo.CurrentHP;
	}

	// �÷��̾��� �ִ� ü���� �ҷ��ɴϴ�.
	FORCEINLINE int GetPlayerMaxHP()
	{
		return PlayerInfo.MaxHP;
	}

	// �÷��̾��� ���ݷ��� �ҷ��ɴϴ�.
	FORCEINLINE int GetPlayerAtk()
	{
		return PlayerInfo.Atk;
	}

	// ��ȣ�ۿ� ������ ���͸� �ҷ��ɴϴ�.
	FORCEINLINE TArray<class UInteractableAreaComponent*> GetInteractableActors()
	{
		return InteractableActors;
	}
};
