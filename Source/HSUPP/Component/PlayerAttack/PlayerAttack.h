// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSUPP.h"
#include "Components/ActorComponent.h"
#include "PlayerAttack.generated.h"


UCLASS()
class HSUPP_API UPlayerAttack : public UActorComponent
{
	GENERATED_BODY()

private:
	// 플레리어의 스킬 정보입니다.
	class UDataTable* DT_SkillInfo;

	// 대기중 / 실행중인 스킬의 정보입니다.
	struct FSkillInfo* WaitSkill;
	struct FSkillInfo* UsingSkill;

	// 이 컴포넌트를 소유할 플레이어 캐릭터입니다.
	class APlayerCharacter* PlayerCharacter;

	// 현재 실행중인 콤보 카운트입니다.
	int ComboCount;

public:	
	// Sets default values for this component's properties
	UPlayerAttack();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 공격 키를 누르면 호출될 메서드
	void TryNormalAttack();

	// 공격을 생성하는 메서드
	void SkillCreator();

	// 공격이 끝날시 사용중인 스킬을 비워주는 메서드
	void ResetUsingSkill();

	// 스킬의 사용도중 그 다음 스킬을 대기열로 추가할수있게 만드는 메서드
	void SetCanWaitSkill();

	// 공격의 실제 타격 범위를 생성할 메서드
	void TraceAttackRange();


	// 사망시 예약된 스킬들을 전부 지울 메서드
	void ResetAllSkill();
	
		
};
