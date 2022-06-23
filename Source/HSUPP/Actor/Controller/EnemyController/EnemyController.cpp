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
	// ���� ������ �����صӴϴ�.
	SetGenericTeamId(ETeam::Enemy);

	// ai�� ����������Ʈ�� �߰����ݴϴ�.
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI_PERCEPTION")));

	// �߰� Ÿ���� �ʱ�ȭ�մϴ�.
	TraceTarget = nullptr;

	// �þ߸� �߰��մϴ�.
	SightSense = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SIGHT_SENSE"));

	// �νİ��� ������ �������ݴϴ�.
	SightSense->SightRadius = 1000.0f;
	SightSense->LoseSightRadius = 2000.0f;
	SightSense->PeripheralVisionAngleDegrees = 180.0f;

	// �ִ� �����ð��� �����մϴ�.
	SightSense->SetMaxAge(2.0f);

	// ���� �÷��̾ �������ְ� �����մϴ�.
	SightSense->DetectionByAffiliation.bDetectEnemies = true;
	SightSense->DetectionByAffiliation.bDetectFriendlies = false;
	SightSense->DetectionByAffiliation.bDetectNeutrals = false;

	// �߰ߵ� ��ġ���� ũ�� ����� �ʾҴٸ� �ڵ����� ���߰��մϴ�.
	SightSense->AutoSuccessRangeFromLastSeenLocation = SightSense->SightRadius;

	// �þ߸� ����������Ʈ�� �����մϴ�.
	GetPerceptionComponent()->ConfigureSense(*SightSense);
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::SearchTraceTarget);


}

void AEnemyController::Tick(float dt)
{
	Super::Tick(dt);

}



void AEnemyController::SearchTraceTarget(AActor* Actor, FAIStimulus Stimulus)
{
	// �þ߰��� ���𰡰� ������ �´ٸ� �߰ݴ���� ���ͷ� �����մϴ�.
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
	//�߰� ����� ������ �߰����� �ʽ��ϴ�.
	if (TraceTarget == nullptr) return false;
	
	// Ÿ���� ��ġ�� �о�ɴϴ�.
	FVector targetVector = TraceTarget->GetActorLocation();

	// �̵��� ��ġ�� ������ ���Ͱ��� ������� �����Դϴ�.
	FVector targetDownVector = targetVector + (FVector::DownVector *1000.0f);

	// ������ �����Դϴ�.
	TArray<AActor*> ignoreActor;

	// ����� ������ �����Դϴ�.
	FHitResult hitResult;

	// �÷��̾��� ��ġ�κ��� �Ʒ��� Ʈ���̽��� �Ͽ� ������ ���Ͱ��� ���ɴϴ�.
	if (UKismetSystemLibrary::LineTraceSingleByProfile(GetWorld(),
		targetVector, targetDownVector, FName(TEXT("SearchPosition")), true, ignoreActor, EDrawDebugTrace::ForDuration, hitResult, true))
	{
		// ���� ��µ� �����ߴٸ� �������������� �������ݴϴ�.
		GetBlackboardComponent()->SetValueAsVector(FName(TEXT("TracePosition")), hitResult.Location);

		return true;
	}


	return false;
}

void AEnemyController::SetAnimWaitTime(float waitTime)
{	
	// ������ �ð���ŭ ���ð��� ���� �������� �Ѿ�� �ʵ��� �մϴ�.
	GetBlackboardComponent()->SetValueAsFloat(FName(TEXT("WaitTime")), waitTime);
}


