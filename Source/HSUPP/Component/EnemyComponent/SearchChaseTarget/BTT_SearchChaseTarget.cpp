// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SearchChaseTarget.h"
	
#include "Actor/Controller/EnemyController/EnemyController.h"


EBTNodeResult::Type UBTT_SearchChaseTarget::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	// �� ��Ʈ�ѷ��� �����մϴ�.
	AEnemyController* enemyController = Cast<AEnemyController>(OwnerComp.GetAIOwner());

	// ��Ʈ�ѷ����� ���� ��ġ�� �����ϰԲ��ϰ� ����� ��ȯ�մϴ�.
	if(enemyController->SetTracePosition())
	{
	 return EBTNodeResult::Succeeded;
	}


	return EBTNodeResult::Failed;
}