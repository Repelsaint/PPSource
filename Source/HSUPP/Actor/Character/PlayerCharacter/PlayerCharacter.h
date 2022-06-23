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
	//캐릭터의 매쉬를 가지고있을 변수
	class USkeletalMesh* CharacterMesh;
	
	// 카메라의 거리를 조정하는 컴포넌트
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UZoomableSpringArm* SpringArm;

	// 카메라 컴포넌트
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	// 유저 정보를 가지고있을 변수
	UPROPERTY()
	FPlayerInfo PlayerInfo;

	// 플레이어 이동 컴포넌트
	UPlayerMovementComponent* PlayerMoveComponent;

	// 플레이어 공격 컴포넌트
	UPlayerAttack* AttackComponent;

	// 플레이어 애니메이션 블루프린트 클래스 
	TSubclassOf<class UPlayerAnimInst> PlayerAnimClass;

	// 상호작용가능한 액터를 저장할 배열
	TArray<class UInteractableAreaComponent*> InteractableActors;

	// 유저의 상태를 나타내는 bool값
	bool bCanMove;
	bool bAlive;

private:
	APlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//공격을 시도하는 메서드
	void TryAttack();


protected:
	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 공격을 받았을때의 처리를 하는 메서드입니다.
	virtual void GetDamaged(int damaged) override;

	// 캐릭터가 죽었을때의 처리를 하는 메서드입니다.
	virtual void CharacterDie() override;

	// 캐릭터의 정보를 불러오는 메서드입니다.
	void LoadPlayerInfo();

	// 플레이어의 사망후 리스폰을 시켜주는 메서드입니다.
	void PlayerRespawn();

	// 상호작용 영역에 들어갔을때 배열에 액터를 추가해주는 메서드입니다.
	void AddInteractableActor(class UInteractableAreaComponent* newActor);

	// 상호작용 영역에서 빠져나왔을때 액터를 배열에서 뺴주는 메서드입니다.
	void RemoveInteractableActor(class UInteractableAreaComponent* deleteActor);

	// 플레이어의 생존상태를 지정하는 메서드입니다.
	FORCEINLINE void SetAlive(bool aliveState)
	{ bAlive = aliveState;}
	
	// 플레이어의 생존상태를 확인할때 사용되는 메서드입니다.
	FORCEINLINE bool IsAlive() const
	{ return bAlive; }

	// 플레이어의 공격컴포넌트를 불러낼때 사용되는 매서드입니다.
	FORCEINLINE UPlayerAttack* GetAttackComponent() const
	{
		return AttackComponent;
	}
	// 플레이어의 스프링암컴포넌트를 불러낼때 사용되는 매서드입니다.
	FORCEINLINE UZoomableSpringArm* GetSpringArm() const
	{
		return SpringArm;
	}
	// 플레이어의 무브컴포넌트를 불러낼때 사용되는 매서드입니다.
	FORCEINLINE UPlayerMovementComponent* GetPlayerMovementComponent() const
	{
		return PlayerMoveComponent;
	}

	// 현재 플레이어가 움직일수있는지 확인하는 메서드입니다.
	FORCEINLINE bool GetCanMove() const
	{
		return bCanMove;
	}

	// 플레이어의 이동 가능상태를 설정할때 사용하는 메서드입니다.
	FORCEINLINE void SetMovable(bool moveType)
	{
		bCanMove = moveType;
	}

	// 플레이어의 정보를 갱신할때 사용하는 메서드입니다.
	FORCEINLINE void SetPlayerInfo(FPlayerInfo playerInfo)
	{
		PlayerInfo = playerInfo;
	}

	// 플레이어의 정보를 읽어들일때 사용하는 메서드입니다.
	// 복사를 이용하면 불필요한 값이 많이 복사되므로 참조를 이용하였습니다.
	FORCEINLINE FPlayerInfo &GetPlayerInfo()
	{
		return PlayerInfo;
	}

	// 플레이어의 현재체력이 변동이 일어났을때 사용되는 메서드입니다.
	FORCEINLINE void AddPlayerCurrentHP(int currentHP) 
	{
		PlayerInfo.CurrentHP += currentHP;
	}

	// 플레이어의 최대체력이 변동이 일어났을때 사용되는 메서드입니다.
	FORCEINLINE void AddPlayerMaxHP(int maxHP)
	{
		PlayerInfo.MaxHP += maxHP;
	}

	// 플레이어의 공격력을 추가할때 사용되는 메서드입니다.
	FORCEINLINE void AddPlayerAtk(int atk)
	{
		PlayerInfo.Atk += atk;
	}

	// 플레이어의 현재 체력을 불러옵니다.
	FORCEINLINE int GetPlayerCurrentHP()
	{
		return PlayerInfo.CurrentHP;
	}

	// 플레이어의 최대 체력을 불러옵니다.
	FORCEINLINE int GetPlayerMaxHP()
	{
		return PlayerInfo.MaxHP;
	}

	// 플레이어의 공격력을 불러옵니다.
	FORCEINLINE int GetPlayerAtk()
	{
		return PlayerInfo.Atk;
	}

	// 상호작용 가능한 액터를 불러옵니다.
	FORCEINLINE TArray<class UInteractableAreaComponent*> GetInteractableActors()
	{
		return InteractableActors;
	}
};
