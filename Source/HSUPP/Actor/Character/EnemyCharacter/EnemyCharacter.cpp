// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "Engine/DataTable.h"

#include "Actor/Controller/EnemyController/EnemyController.h"	
#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"
#include "Actor/EnemySpawnArea/EnemySpawnArea.h"
#include "Actor/NormalLevelActor/NormalLevelActor.h"

#include "Widget/MainWidget.h"

#include "Kismet/KismetSystemLibrary.h"

#include "BehaviorTree/BehaviorTree.h"

#include "Anim/EnemyAnimInstance/EnemyAnimInstance.h"

#include "Struct/EnemyInfo.h"
#include "Struct/PlayerInfo.h"

AEnemyCharacter::AEnemyCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	// �� ������ �ҷ����Դϴ�.
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ENEMY_INFO(TEXT("DataTable'/Game/DataTable/DT_EnemyInfo.DT_EnemyInfo'"));
	if(DT_ENEMY_INFO.Succeeded())
		DT_EnemyInfo = DT_ENEMY_INFO.Object;

	// AI�� ��Ʈ�ѷ�Ŭ������ �ۼ��� ��Ʈ�ѷ� Ŭ������ �������ݴϴ�.
	AIControllerClass = AEnemyController::StaticClass();
	
	// ��Ʈ�ѷ� ���� ����� �����մϴ�.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	// �� ������ �ʱ�ȭ �س����ϴ�.
	EnemyCode = 0;
	BehaviorTree = nullptr;
	AttackAnim = nullptr;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyCharacter::UnPossessed()
{
	Super::UnPossessed();
}

void AEnemyCharacter::GetDamaged(int damaged)
{
	Super::GetDamaged(damaged);

	// ���� �������� �԰� ü���� 0 ���Ϸ� ������������ ó���Դϴ�.
	if (CurrentHP <= 0)
	{
		// �÷��̾��� ������ ���� ����ϴ� ����ġ�� ��带 �߰����ݴϴ�.
		FPlayerInfo& playerInfo = GetHInstance()->GetPlayerCharacter()->GetPlayerInfo();
		playerInfo.Money += DropGold;
		playerInfo.Exp += DropExp;

		// ���� ���� ����ġ�� �÷��̾��� ����ġ �������ɷ����� ���ٸ� ����ġ�� ��ġ�� ���������� ������ �÷��ݴϴ�.
		for(int i = 0; (playerInfo.Exp >= (playerInfo.Level * 10)); ++i)
		{
			playerInfo.Exp -= (playerInfo.Level*10);
			playerInfo.Level += 1;
		}
		// ���� ���̰� �÷��̾��� ���� ���� ��ȭ�� �������Ƿ� ������ �������ݴϴ�.
		GetHInstance()->GetMainWidget()->SetCharacterDataText(playerInfo.Level,playerInfo.Exp,playerInfo.CharacterName);
	}

}



void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyCharacter::SetEnemyCode(int enemyCode)
{
	// �ʱ� ���ʹ� ���� �ʱ�ȭ���Դϴ�.
	EnemyCode = enemyCode;
}

void AEnemyCharacter::InitializeEnemyCharacter()
{
	// ���� �� �ڵ尡 ����� �������� �ʴٸ� ���������ʽ��ϴ�.
	if(EnemyCode == 0)
	 return;

	 // �� ���̵� ������ �����մϴ�.
	SetGenericTeamId(ETeam::Enemy);

	//�����ڵ带 �̿��Ͽ� ���� ������ �о���Դϴ�.
	FName enemyCodeName = FName(*FString::FromInt(EnemyCode));
	FEnemyInfo* MonsterInfo = (DT_EnemyInfo->FindRow<FEnemyInfo>(enemyCodeName,TEXT("")));

	// ���� �⺻ ������ �����մϴ�.
	CurrentHP = MaxHP = MonsterInfo->MaxHP;
	Atk = MonsterInfo->Atk;
	DropExp = MonsterInfo->DropExp;
	DropGold = MonsterInfo->DropGold;

	// ���� �°ų� ������ �߻��ϴ� �Ҹ��� ��ƼŬ�� �ҷ��ɴϴ�.
	HitParticle = Cast<UParticleSystem>(GetStreamManager()->LoadSynchronous(MonsterInfo->HitParticle));
	AttackSound = Cast<USoundWave>(GetStreamManager()->LoadSynchronous(MonsterInfo->AttackSound));
	HitSound = Cast<USoundWave>(GetStreamManager()->LoadSynchronous(MonsterInfo->HitSound));

	// �����Ҷ� �����ų �ִԸ�Ÿ�긦 �ҷ��ɴϴ�.
	AttackAnim = Cast<UAnimMontage>(GetStreamManager()->LoadSynchronous(MonsterInfo->AnimMontagePath));
	
	// ���� �ӵ�, �������� �⺻ ������ �ʱ�ȭ�մϴ�.
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetMesh()->SetSkeletalMesh(Cast<USkeletalMesh>(GetStreamManager()->LoadSynchronous(MonsterInfo->EnemyMeshPath)));
	GetMesh()->SetRelativeLocationAndRotation(FVector::DownVector * 88.0f, FRotator(0.0f, -90.0f, 0.0f));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyCharacter"));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	// �����̺�� Ʈ���� �ҷ��ɴϴ�.
	FSoftObjectPath behaviorTreePath = TEXT("/Game/AI/BH_Enemy.BH_Enemy");
	BehaviorTree = Cast<UBehaviorTree>(GetStreamManager()->LoadSynchronous(behaviorTreePath));

	// ������ �ҷ� �Դٸ� ���� ��Ʈ�ѷ��� �������ݴϴ�.
	if (IsValid(BehaviorTree))
		Cast<AEnemyController>(GetController())->RunBehaviorTree(BehaviorTree);

		
	// �ִϸ��̼� ������ �� �Ŀ� ���� ���ü��� �������ݴϴ�.
	GetMesh()->SetAnimInstanceClass(nullptr);
	GetMesh()->SetAnimInstanceClass(MonsterInfo->EnemyAnimClass);
	GetMesh()->SetVisibility(true);

}



void AEnemyCharacter::CharacterDie()
{
	Super::CharacterDie();

	// ĳ���Ͱ� ������ ���� ��� �ൿ�� ���߰� ���� ���� bool ���� false�� �������ݴϴ�.
	Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance())->StopAllMontages(0.0f);
	Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance())->ChangeAliveState(false);
}

void AEnemyCharacter::EnemyRespawn()
{
	//����� ���� ���� ��ġ�� ����ϴ� �����ڽ��� ã�Ƽ� �������� �����ݴϴ�.
	Cast<ANormalLevelActor>(GetWorld()->GetLevelScriptActor())->GetSpawnAreaArray(EnemyCode)->RespawnEnemy(this);
}

bool AEnemyCharacter::TryEnemyAttack()
{
	// ������ �׾��ִٸ� ������ ���� �ʽ��ϴ�.
	if(!GetHInstance()->GetPlayerCharacter()->IsAlive()) return false;

	// �������� �Ÿ��� Ȯ���ϰ� ������ �̹� �ָ� ������ �Ķ�� ������ ���� �ʽ��ϴ�.
	float distance = FVector::Distance(GetHInstance()->GetPlayerCharacter()->GetActorLocation(),GetActorLocation());
	if(distance > 400.0f) return false;

	// ����ݽð��� ���ؼ� �ִԸ�Ÿ���� ���ӽð��� �������� �� �ð��� �����ڿ� ���ൿ�� �����ϰԲ� ����Ӵϴ�.
	float durationTime = PlayAnimMontage(AttackAnim,1.0f,FName(TEXT("EnemyAttack")));
	durationTime -= 0.5f;
	Cast<AEnemyController>(GetController())->SetAnimWaitTime(durationTime);

	return true;

}


bool AEnemyCharacter::EnemyAttack()
{

	// ���ݽ� ������ �����Դϴ�.
	TArray<AActor*> ignoreActor;

	// ���ݽ� ������ ������Դϴ�.
	TArray<FHitResult> hitResults;

	// �迭�� �ʱ�ȭ���ݴϴ�.
	ignoreActor.Empty();

	//���� ������ ���α��� ������ 200��ŭ �����ϰ� �����մϴ�.
	FVector startVector = GetActorLocation();
	FVector endVector = startVector + (GetActorForwardVector()* 200.0f);

	// ���� ������ �˸��� ���ݽõ� �Ҹ��� ���Բ� �մϴ�.
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), AttackSound,GetActorLocation());

	// ���Ǿ� Ʈ���̽��� ���� ���� ����� �����س��ϴ�.
	if (!UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(),
	startVector, endVector, 70.0f, FName(TEXT("TryAttack")), true, ignoreActor, EDrawDebugTrace::ForDuration, hitResults,true))
	{
		// hitResult�� �ɸ� ������Ʈ ����ŭ �ݺ��մϴ�.
		for (FHitResult hit : hitResults)
		{
			// �ɸ� ������ ���̵� �÷��̾��� ��쿡 ������ó�����մϴ�.
			if (Cast<ABaseCharacter>(hit.Actor)->GetGenericTeamId() == ETeam::Player)
			{
				// �÷��̾��� GetDamaged�޼���� ���ݷ¸�ŭ �÷��̾��� ü���� ����ϴ�.
				Cast<APlayerCharacter>(hit.Actor)->GetDamaged(GetEnemyAtk());

				// ����� ��ġ�� ���� hit Particle�� ��ȯ�Ͽ��ݴϴ�.
				Cast<ANormalLevelActor>(GetWorld()->GetLevelScriptActor())->SpawnParticle(HitParticle, hit.ImpactPoint);

				// ���������� �Ҹ��� ������ݴϴ�.
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());


				return true;
			}
		}
		
	}



	return false;
}

void AEnemyCharacter::SetEnemyWalkSpeed(float walkSpeed)
{
	// ���� �̵��ӵ��� �����մϴ�.
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}
