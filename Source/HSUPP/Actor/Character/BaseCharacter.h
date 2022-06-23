// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSUPP.h"
#include "GameFramework/Character.h"
#include "Enum/Team.h"
#include "GenericTeamAgentInterface.h"

#include "BaseCharacter.generated.h"

UCLASS()
class HSUPP_API ABaseCharacter : public ACharacter,
	public IGenericTeamAgentInterface
{
	GENERATED_BODY()

protected:
	//팀 구분용 변수입니다.
	FGenericTeamId Team;

	// 캐릭터의 기본 정보
	int CurrentHP;
	int MaxHP;
	int Atk;

public:
	// Sets default values for this character's properties
	ABaseCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 데미지를 받았을때 호출될 메서드입니다.
	virtual void GetDamaged(int damaged);

	// 캐릭터가 죽었을때 호출될 메서드입니다.
	virtual void CharacterDie();

	// 팀설정메서드입니다.
	FORCEINLINE virtual void SetGenericTeamId(const FGenericTeamId& TeamID)
	{
		Team = TeamID;
	}

	// 팀을 확인할때 사용될 메서드입니다.
	FORCEINLINE virtual FGenericTeamId GetGenericTeamId() const
	{
		return Team;
	}

};
