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
	// �÷������� ��ų �����Դϴ�.
	class UDataTable* DT_SkillInfo;

	// ����� / �������� ��ų�� �����Դϴ�.
	struct FSkillInfo* WaitSkill;
	struct FSkillInfo* UsingSkill;

	// �� ������Ʈ�� ������ �÷��̾� ĳ�����Դϴ�.
	class APlayerCharacter* PlayerCharacter;

	// ���� �������� �޺� ī��Ʈ�Դϴ�.
	int ComboCount;

public:	
	// Sets default values for this component's properties
	UPlayerAttack();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ���� Ű�� ������ ȣ��� �޼���
	void TryNormalAttack();

	// ������ �����ϴ� �޼���
	void SkillCreator();

	// ������ ������ ������� ��ų�� ����ִ� �޼���
	void ResetUsingSkill();

	// ��ų�� ��뵵�� �� ���� ��ų�� ��⿭�� �߰��Ҽ��ְ� ����� �޼���
	void SetCanWaitSkill();

	// ������ ���� Ÿ�� ������ ������ �޼���
	void TraceAttackRange();


	// ����� ����� ��ų���� ���� ���� �޼���
	void ResetAllSkill();
	
		
};
