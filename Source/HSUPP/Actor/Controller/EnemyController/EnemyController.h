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
	// �þ� ���� �����Դϴ�.
	UPROPERTY(VisibleAnywhere)
	class UAISenseConfig_Sight* SightSense;

	// �߰��� �����Դϴ�.
	class AActor* TraceTarget;



public:
	AEnemyController();

	virtual void Tick(float dt) override;

	// �߰��� ����� ã�� �޼����Դϴ�.
	UFUNCTION()
	void SearchTraceTarget(AActor* Actor, FAIStimulus Stimulus);

	// �߰��� ��ġ�� �����մϴ�.
	bool SetTracePosition();

	// �ִϸ��̼��� ���ð��� �����ϴ� �޼����Դϴ�.
	void SetAnimWaitTime(float waitTime);




};
