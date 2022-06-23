// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAttack.h"

#include "Engine/DataTable.h"

#include "Struct/SkillInfo.h"
#include "Struct/SkillDamageInfo.h"

#include "Kismet/KismetSystemLibrary.h"

#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"
#include "Actor/Character/EnemyCharacter/EnemyCharacter.h"
#include "Actor/NormalLevelActor/NormalLevelActor.h"

// Sets default values for this component's properties
UPlayerAttack::UPlayerAttack()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// 스킬 정보를 불러들입니다.
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_SKILL_INFO(TEXT("DataTable'/Game/DataTable/DT_PCSkill.DT_PCSkill'"));
	if (DT_SKILL_INFO.Succeeded())
		DT_SkillInfo = DT_SKILL_INFO.Object;

		// 현재 진행중인 공격 카운트를 초기화합니다.
	ComboCount = 0;
}


// Called when the game starts
void UPlayerAttack::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어 캐릭터를 지정합니다.
	PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	
}


// Called every frame
void UPlayerAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 저장된  스킬이 있으면  소환해주는 메서드입니다.
	SkillCreator();
}

void UPlayerAttack::TryNormalAttack()
{
	// 이미 대기중인 스킬 혹은 공격이 있다면 실행을 취소합니다.
	if (WaitSkill != nullptr) return;
		WaitSkill = DT_SkillInfo->FindRow<FSkillInfo>(TEXT("NormalAttack"), TEXT(""));

}

void UPlayerAttack::SkillCreator()
{
	// 실행중인 스킬이 존재한다면 스킬생성을 하지않습니다.
	if (UsingSkill != nullptr) return;

	// 대기중인 스킬이 존재하지 않는다면 실행하지않습니다.
	if (WaitSkill == nullptr) return;


	
	// 대기중이던 스킬을 실행중으로 옮깁니다.
	UsingSkill = WaitSkill;

	UAnimMontage* skillAnimMontage = Cast<UAnimMontage>(GetStreamManager()->LoadSynchronous(UsingSkill->AnimMontagePath));

	// 콤보공격의 경우 지정된 위치에서 시작될수 있도록 하는 변수입니다.
	FName SectionName;

	// 진행중인 콤보가 없다면 섹션네임을 지정하지않습니다.
	if (UsingSkill->ComboAttackName == TEXT("NONE"))
		SectionName = TEXT("");
	// 콤보중이라면 섹션네임을 지정해줍니다.
	else
	{
		SectionName = FName(*(UsingSkill->ComboAttackName + TEXT("_") + FString::FromInt(ComboCount)));
	}



	// 실행할 애니메이션을 로드하고 실행합니다.
	PlayerCharacter->PlayAnimMontage(skillAnimMontage, 1.0f, SectionName);
	
	

}

void UPlayerAttack::ResetUsingSkill()
{
	// 대기중인 스킬이 없거나
	// 대기중이지만 현재스킬과는 다른 스킬을 시전하려고 하거나
	// 이미 최대스킬콤보를 사용한 경우에는 콤보카운트를 0으로 지정합니다.
	if (WaitSkill == nullptr || UsingSkill != WaitSkill || ( (UsingSkill->SkillDamage.Num()-1) <= ComboCount) )
	{
		ComboCount = 0;
	}
	else if (UsingSkill == WaitSkill)
	{
		++ComboCount;
	}
	UsingSkill = nullptr;
}

void UPlayerAttack::SetCanWaitSkill()
{
	WaitSkill = nullptr;
}

void UPlayerAttack::TraceAttackRange()
{
	// 데미지 계산을 위한 기본공격력 설정
	float ATK = PlayerCharacter->GetPlayerAtk();

	// 계산식 처리를위한 중간 데미지와 최종데미지변수
	float SemiDamage = 0.0f;
	float EndDamage = 0.0f;

	// 발동할 스킬 콤보 정보
	FSkillDamageInfo DamageInfo = UsingSkill->SkillDamage[ComboCount];

	// 현재 스킬 콤보의 데미지 공식
	FString DamageF = DamageInfo.DamageFormula;


	// FString으로된 데미지식 해제하기
	for (int i = 0; i < DamageF.Len(); ++i)
	{
		if (DamageF[i] == TEXT('+'))
		{
			EndDamage += SemiDamage;
			SemiDamage = 0.0f;
		}
		else if (DamageF[i] == TEXT('%'))
		{
			SemiDamage *= ATK;
			SemiDamage *= 0.01f;
			EndDamage += SemiDamage;
			SemiDamage = 0.0f;
		}
		else if(DamageF[i] == TEXT(' '))
		{ 
		// 실수나 모종의 이유로 빈칸을 배치했을경우 그냥 넘기기 위함
		}
		else
		{
			SemiDamage *= 10.0f;
			SemiDamage += ((float)(DamageF[i])-48.0f);
		}
	}

	// 계산이 끝났으므로 엔드데미지를 지정해줍니다.
	if (SemiDamage > 0.0f)
	{
		EndDamage += SemiDamage;
		SemiDamage = 0.0f;
	}

	// 스킬 정보를 토대로 공격시작 범위와 공격 종료범위를 지정합니다.
	FVector TraceStartVector = GetOwner()->GetActorLocation() + DamageInfo.SkillStartVector;
	FVector TraceEndVector = TraceStartVector + (DamageInfo.GetTraceDirection(GetOwner()) * DamageInfo.TraceDistance);

	// 검사시 무시할 객체를 지정합니다.
	TArray<AActor*> IgnoreActor;
	IgnoreActor.Add(GetOwner());

	// 트레이싱 결과를 저장할 배열
	TArray<FHitResult> hitResults;

	// 현재 공격 콤보에 맞는 소리가 지정되어있다면 지정된 소리로 그렇지 않다면 마지막 콤보소리가 나오게 설정합니다. 
	int currentSoundNum = (UsingSkill->UseSoundWave.Num() > ComboCount) ? ComboCount : (UsingSkill->UseSoundWave.Num()-1);
	USoundWave* attackSound = Cast<USoundWave>(GetStreamManager()->LoadSynchronous(UsingSkill->UseSoundWave[currentSoundNum]));
	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), attackSound,GetOwner()->GetActorLocation());

	if (!UKismetSystemLibrary::SphereTraceMultiByProfile
	(GetWorld(), TraceStartVector, TraceEndVector, DamageInfo.TraceRange, TEXT("TryAttack"), true, IgnoreActor, EDrawDebugTrace::ForDuration, hitResults, true))
	{	
		for (int i = 0; i < hitResults.Num(); ++i)
		{
			//공격에 적이 맞지 않았다면 실행하지않습니다.
			if(Cast<AEnemyCharacter>(hitResults[i].Actor) == nullptr) return;
			// 맞은액터가 적이라면 데미지를 입힙니다.
			Cast<AEnemyCharacter>(hitResults[i].Actor)->GetDamaged(EndDamage);
			// 적의 위치에 파티클을 생성합니다.
			Cast<ANormalLevelActor>(GetWorld()->GetLevelScriptActor())->SpawnParticle(Cast<UParticleSystem>(GetStreamManager()->LoadSynchronous(UsingSkill->HitParticle)), hitResults[i].ImpactPoint);

			// 타격위치에 소리를 실행시켜줍니다.
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), Cast<USoundWave>(GetStreamManager()->LoadSynchronous(UsingSkill->HitSoundWave))
			, hitResults[i].ImpactPoint);
		}

	}


}

void UPlayerAttack::ResetAllSkill()
{	
	// 모든 공격 정보를 초기화합니다.
	ComboCount = 0;
	WaitSkill = nullptr;
	UsingSkill = nullptr;
}

