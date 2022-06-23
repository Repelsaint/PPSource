// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSUPP.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_SearchChaseTarget.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API UBTT_SearchChaseTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()


public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
