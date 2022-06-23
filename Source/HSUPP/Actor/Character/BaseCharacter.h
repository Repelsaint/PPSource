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
	//�� ���п� �����Դϴ�.
	FGenericTeamId Team;

	// ĳ������ �⺻ ����
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

	// �������� �޾����� ȣ��� �޼����Դϴ�.
	virtual void GetDamaged(int damaged);

	// ĳ���Ͱ� �׾����� ȣ��� �޼����Դϴ�.
	virtual void CharacterDie();

	// �������޼����Դϴ�.
	FORCEINLINE virtual void SetGenericTeamId(const FGenericTeamId& TeamID)
	{
		Team = TeamID;
	}

	// ���� Ȯ���Ҷ� ���� �޼����Դϴ�.
	FORCEINLINE virtual FGenericTeamId GetGenericTeamId() const
	{
		return Team;
	}

};
