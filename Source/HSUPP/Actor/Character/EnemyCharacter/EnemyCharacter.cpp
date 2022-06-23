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

	// 적 정보를 불러들입니다.
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ENEMY_INFO(TEXT("DataTable'/Game/DataTable/DT_EnemyInfo.DT_EnemyInfo'"));
	if(DT_ENEMY_INFO.Succeeded())
		DT_EnemyInfo = DT_ENEMY_INFO.Object;

	// AI의 컨트롤러클래스를 작성한 컨트롤러 클래스로 변경해줍니다.
	AIControllerClass = AEnemyController::StaticClass();
	
	// 컨트롤러 생성 방식을 정의합니다.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	// 적 정보를 초기화 해놓습니다.
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

	// 적이 데미지를 입고 체력이 0 이하로 떨어졌을때의 처리입니다.
	if (CurrentHP <= 0)
	{
		// 플레이어의 정보에 적이 드랍하는 경험치와 골드를 추가해줍니다.
		FPlayerInfo& playerInfo = GetHInstance()->GetPlayerCharacter()->GetPlayerInfo();
		playerInfo.Money += DropGold;
		playerInfo.Exp += DropExp;

		// 만약 들어온 경험치가 플레이어의 경험치 보유가능량보다 높다면 경험치가 넘치지 않을때까지 레벨을 올려줍니다.
		for(int i = 0; (playerInfo.Exp >= (playerInfo.Level * 10)); ++i)
		{
			playerInfo.Exp -= (playerInfo.Level*10);
			playerInfo.Level += 1;
		}
		// 적을 죽이고 플레이어의 레벨 등의 변화가 생겼으므로 정보를 수정해줍니다.
		GetHInstance()->GetMainWidget()->SetCharacterDataText(playerInfo.Level,playerInfo.Exp,playerInfo.CharacterName);
	}

}



void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyCharacter::SetEnemyCode(int enemyCode)
{
	// 초기 에너미 정보 초기화용입니다.
	EnemyCode = enemyCode;
}

void AEnemyCharacter::InitializeEnemyCharacter()
{
	// 만약 적 코드가 제대로 들어와있지 않다면 실행하지않습니다.
	if(EnemyCode == 0)
	 return;

	 // 팀 아이디를 적으로 지정합니다.
	SetGenericTeamId(ETeam::Enemy);

	//고유코드를 이용하여 적의 정보를 읽어들입니다.
	FName enemyCodeName = FName(*FString::FromInt(EnemyCode));
	FEnemyInfo* MonsterInfo = (DT_EnemyInfo->FindRow<FEnemyInfo>(enemyCodeName,TEXT("")));

	// 적의 기본 정보를 리셋합니다.
	CurrentHP = MaxHP = MonsterInfo->MaxHP;
	Atk = MonsterInfo->Atk;
	DropExp = MonsterInfo->DropExp;
	DropGold = MonsterInfo->DropGold;

	// 적이 맞거나 때릴때 발생하는 소리와 파티클을 불러옵니다.
	HitParticle = Cast<UParticleSystem>(GetStreamManager()->LoadSynchronous(MonsterInfo->HitParticle));
	AttackSound = Cast<USoundWave>(GetStreamManager()->LoadSynchronous(MonsterInfo->AttackSound));
	HitSound = Cast<USoundWave>(GetStreamManager()->LoadSynchronous(MonsterInfo->HitSound));

	// 공격할때 실행시킬 애님몽타쥬를 불러옵니다.
	AttackAnim = Cast<UAnimMontage>(GetStreamManager()->LoadSynchronous(MonsterInfo->AnimMontagePath));
	
	// 적의 속도, 외형등의 기본 정보를 초기화합니다.
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetMesh()->SetSkeletalMesh(Cast<USkeletalMesh>(GetStreamManager()->LoadSynchronous(MonsterInfo->EnemyMeshPath)));
	GetMesh()->SetRelativeLocationAndRotation(FVector::DownVector * 88.0f, FRotator(0.0f, -90.0f, 0.0f));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyCharacter"));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	// 비헤이비어 트리를 불러옵니다.
	FSoftObjectPath behaviorTreePath = TEXT("/Game/AI/BH_Enemy.BH_Enemy");
	BehaviorTree = Cast<UBehaviorTree>(GetStreamManager()->LoadSynchronous(behaviorTreePath));

	// 무사히 불러 왔다면 적의 컨트롤러에 적용해줍니다.
	if (IsValid(BehaviorTree))
		Cast<AEnemyController>(GetController())->RunBehaviorTree(BehaviorTree);

		
	// 애니메이션 설정을 한 후에 적의 가시성을 변경해줍니다.
	GetMesh()->SetAnimInstanceClass(nullptr);
	GetMesh()->SetAnimInstanceClass(MonsterInfo->EnemyAnimClass);
	GetMesh()->SetVisibility(true);

}



void AEnemyCharacter::CharacterDie()
{
	Super::CharacterDie();

	// 캐릭터가 죽으면 적의 모든 행동을 멈추고 생존 관련 bool 값을 false로 변경해줍니다.
	Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance())->StopAllMontages(0.0f);
	Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance())->ChangeAliveState(false);
}

void AEnemyCharacter::EnemyRespawn()
{
	//사망한 적의 생성 위치를 담당하는 스폰박스를 찾아서 리스폰을 시켜줍니다.
	Cast<ANormalLevelActor>(GetWorld()->GetLevelScriptActor())->GetSpawnAreaArray(EnemyCode)->RespawnEnemy(this);
}

bool AEnemyCharacter::TryEnemyAttack()
{
	// 유저가 죽어있다면 공격을 하지 않습니다.
	if(!GetHInstance()->GetPlayerCharacter()->IsAlive()) return false;

	// 유저와의 거리를 확인하고 유저가 이미 멀리 도망간 후라면 공격을 하지 않습니다.
	float distance = FVector::Distance(GetHInstance()->GetPlayerCharacter()->GetActorLocation(),GetActorLocation());
	if(distance > 400.0f) return false;

	// 재공격시간을 위해서 애님몽타쥬의 지속시간을 가져오고 그 시간이 지난뒤에 재행동이 가능하게끔 멈춰둡니다.
	float durationTime = PlayAnimMontage(AttackAnim,1.0f,FName(TEXT("EnemyAttack")));
	durationTime -= 0.5f;
	Cast<AEnemyController>(GetController())->SetAnimWaitTime(durationTime);

	return true;

}


bool AEnemyCharacter::EnemyAttack()
{

	// 공격시 무시할 액터입니다.
	TArray<AActor*> ignoreActor;

	// 공격시 적중한 결과물입니다.
	TArray<FHitResult> hitResults;

	// 배열을 초기화해줍니다.
	ignoreActor.Empty();

	//공격 범위를 본인기준 앞으로 200만큼 공격하게 지시합니다.
	FVector startVector = GetActorLocation();
	FVector endVector = startVector + (GetActorForwardVector()* 200.0f);

	// 적의 외형에 알맞은 공격시도 소리가 나게끔 합니다.
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), AttackSound,GetActorLocation());

	// 스피어 트레이스를 통해 어택 결과를 도출해냅니다.
	if (!UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(),
	startVector, endVector, 70.0f, FName(TEXT("TryAttack")), true, ignoreActor, EDrawDebugTrace::ForDuration, hitResults,true))
	{
		// hitResult에 걸린 오브젝트 수만큼 반복합니다.
		for (FHitResult hit : hitResults)
		{
			// 걸린 액터의 아이디가 플레이어일 경우에 데미지처리를합니다.
			if (Cast<ABaseCharacter>(hit.Actor)->GetGenericTeamId() == ETeam::Player)
			{
				// 플레이어의 GetDamaged메서드로 공격력만큼 플레이어의 체력을 깎습니다.
				Cast<APlayerCharacter>(hit.Actor)->GetDamaged(GetEnemyAtk());

				// 맞췄던 위치에 적의 hit Particle을 소환하여줍니다.
				Cast<ANormalLevelActor>(GetWorld()->GetLevelScriptActor())->SpawnParticle(HitParticle, hit.ImpactPoint);

				// 맞췄을때의 소리를 출력해줍니다.
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());


				return true;
			}
		}
		
	}



	return false;
}

void AEnemyCharacter::SetEnemyWalkSpeed(float walkSpeed)
{
	// 적의 이동속도를 조정합니다.
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}
