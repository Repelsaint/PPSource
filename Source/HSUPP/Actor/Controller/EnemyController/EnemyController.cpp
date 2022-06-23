// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "Enum/Team.h"

#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"
#include "Actor/Character/EnemyCharacter/EnemyCharacter.h"


AEnemyController::AEnemyController()
{
	// 팀을 적으로 지정해둡니다.
	SetGenericTeamId(ETeam::Enemy);

	// ai의 감각컴포넌트를 추가해줍니다.
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI_PERCEPTION")));

	// 추격 타겟을 초기화합니다.
	TraceTarget = nullptr;

	// 시야를 추가합니다.
	SightSense = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SIGHT_SENSE"));

	// 인식관련 정보를 지정해줍니다.
	SightSense->SightRadius = 1000.0f;
	SightSense->LoseSightRadius = 2000.0f;
	SightSense->PeripheralVisionAngleDegrees = 180.0f;

	// 최대 유지시간을 지정합니다.
	SightSense->SetMaxAge(2.0f);

	// 적이 플레이어만 쫓을수있게 지정합니다.
	SightSense->DetectionByAffiliation.bDetectEnemies = true;
	SightSense->DetectionByAffiliation.bDetectFriendlies = false;
	SightSense->DetectionByAffiliation.bDetectNeutrals = false;

	// 발견된 위치에서 크게 벗어나지 않았다면 자동으로 재추격합니다.
	SightSense->AutoSuccessRangeFromLastSeenLocation = SightSense->SightRadius;

	// 시야를 감각컴포넌트에 적용합니다.
	GetPerceptionComponent()->ConfigureSense(*SightSense);
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::SearchTraceTarget);


}

void AEnemyController::Tick(float dt)
{
	Super::Tick(dt);

}



void AEnemyController::SearchTraceTarget(AActor* Actor, FAIStimulus Stimulus)
{
	// 시야각에 무언가가 잡힌게 맞다면 추격대상을 액터로 지정합니다.
	if (Stimulus.WasSuccessfullySensed())
	{
		TraceTarget = Actor;
	}
	else
	{
		TraceTarget = nullptr;
	}

}

bool AEnemyController::SetTracePosition()
{
	//추격 대상이 없으면 추격하지 않습니다.
	if (TraceTarget == nullptr) return false;
	
	// 타겟의 위치를 읽어옵니다.
	FVector targetVector = TraceTarget->GetActorLocation();

	// 이동할 위치의 지면의 벡터값을 얻기위한 변수입니다.
	FVector targetDownVector = targetVector + (FVector::DownVector *1000.0f);

	// 무시할 액터입니다.
	TArray<AActor*> ignoreActor;

	// 결과를 저장할 변수입니다.
	FHitResult hitResult;

	// 플레이어의 위치로부터 아래로 트레이스를 하여 지면의 벡터값을 얻어옵니다.
	if (UKismetSystemLibrary::LineTraceSingleByProfile(GetWorld(),
		targetVector, targetDownVector, FName(TEXT("SearchPosition")), true, ignoreActor, EDrawDebugTrace::ForDuration, hitResult, true))
	{
		// 값을 얻는데 성공했다면 추적포지션으로 설정해줍니다.
		GetBlackboardComponent()->SetValueAsVector(FName(TEXT("TracePosition")), hitResult.Location);

		return true;
	}


	return false;
}

void AEnemyController::SetAnimWaitTime(float waitTime)
{	
	// 지정한 시간만큼 대기시간을 만들어서 다음으로 넘어가지 않도록 합니다.
	GetBlackboardComponent()->SetValueAsFloat(FName(TEXT("WaitTime")), waitTime);
}


