// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_EnemyAttack.h"

#include "Actor/Controller/EnemyController/EnemyController.h"
#include "Actor/Character/EnemyCharacter/EnemyCharacter.h"


EBTNodeResult::Type UBTT_EnemyAttack::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	// �������� ��Ʈ�ѷ��� �����մϴ�.
	AEnemyController* enemyController = Cast<AEnemyController>(OwnerComp.GetAIOwner());

	// ������ �õ��ϰ� ���� �������θ� ��ȯ�մϴ�.
	if (Cast<AEnemyCharacter>(enemyController->GetPawn())->TryEnemyAttack())
	{
		return EBTNodeResult::Succeeded;
	}


	return EBTNodeResult::Failed;
}