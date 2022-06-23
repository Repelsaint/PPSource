// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SearchChaseTarget.h"
	
#include "Actor/Controller/EnemyController/EnemyController.h"


EBTNodeResult::Type UBTT_SearchChaseTarget::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	// 적 컨트롤러를 저장합니다.
	AEnemyController* enemyController = Cast<AEnemyController>(OwnerComp.GetAIOwner());

	// 컨트롤러에서 적의 위치를 지정하게끔하고 결과를 반환합니다.
	if(enemyController->SetTracePosition())
	{
	 return EBTNodeResult::Succeeded;
	}


	return EBTNodeResult::Failed;
}