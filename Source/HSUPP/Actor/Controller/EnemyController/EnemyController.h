// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"

#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API AEnemyController : public AAIController
{
	GENERATED_BODY()
	
private:
	// 시야 감각 변수입니다.
	UPROPERTY(VisibleAnywhere)
	class UAISenseConfig_Sight* SightSense;

	// 추격할 액터입니다.
	class AActor* TraceTarget;



public:
	AEnemyController();

	virtual void Tick(float dt) override;

	// 추격할 대상을 찾는 메서드입니다.
	UFUNCTION()
	void SearchTraceTarget(AActor* Actor, FAIStimulus Stimulus);

	// 추격할 위치를 지정합니다.
	bool SetTracePosition();

	// 애니메이션의 대기시간을 지정하는 메서드입니다.
	void SetAnimWaitTime(float waitTime);




};
