// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_EnemyAttack.h"

#include "Actor/Controller/EnemyController/EnemyController.h"
#include "Actor/Character/EnemyCharacter/EnemyCharacter.h"


EBTNodeResult::Type UBTT_EnemyAttack::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	// 소유중인 컨트롤러를 저장합니다.
	AEnemyController* enemyController = Cast<AEnemyController>(OwnerComp.GetAIOwner());

	// 공격을 시도하고 공격 성공여부를 반환합니다.
	if (Cast<AEnemyCharacter>(enemyController->GetPawn())->TryEnemyAttack())
	{
		return EBTNodeResult::Succeeded;
	}


	return EBTNodeResult::Failed;
}