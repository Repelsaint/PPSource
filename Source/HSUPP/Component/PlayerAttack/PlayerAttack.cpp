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

	// ��ų ������ �ҷ����Դϴ�.
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_SKILL_INFO(TEXT("DataTable'/Game/DataTable/DT_PCSkill.DT_PCSkill'"));
	if (DT_SKILL_INFO.Succeeded())
		DT_SkillInfo = DT_SKILL_INFO.Object;

		// ���� �������� ���� ī��Ʈ�� �ʱ�ȭ�մϴ�.
	ComboCount = 0;
}


// Called when the game starts
void UPlayerAttack::BeginPlay()
{
	Super::BeginPlay();

	// �÷��̾� ĳ���͸� �����մϴ�.
	PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	
}


// Called every frame
void UPlayerAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// �����  ��ų�� ������  ��ȯ���ִ� �޼����Դϴ�.
	SkillCreator();
}

void UPlayerAttack::TryNormalAttack()
{
	// �̹� ������� ��ų Ȥ�� ������ �ִٸ� ������ ����մϴ�.
	if (WaitSkill != nullptr) return;
		WaitSkill = DT_SkillInfo->FindRow<FSkillInfo>(TEXT("NormalAttack"), TEXT(""));

}

void UPlayerAttack::SkillCreator()
{
	// �������� ��ų�� �����Ѵٸ� ��ų������ �����ʽ��ϴ�.
	if (UsingSkill != nullptr) return;

	// ������� ��ų�� �������� �ʴ´ٸ� ���������ʽ��ϴ�.
	if (WaitSkill == nullptr) return;


	
	// ������̴� ��ų�� ���������� �ű�ϴ�.
	UsingSkill = WaitSkill;

	UAnimMontage* skillAnimMontage = Cast<UAnimMontage>(GetStreamManager()->LoadSynchronous(UsingSkill->AnimMontagePath));

	// �޺������� ��� ������ ��ġ���� ���۵ɼ� �ֵ��� �ϴ� �����Դϴ�.
	FName SectionName;

	// �������� �޺��� ���ٸ� ���ǳ����� ���������ʽ��ϴ�.
	if (UsingSkill->ComboAttackName == TEXT("NONE"))
		SectionName = TEXT("");
	// �޺����̶�� ���ǳ����� �������ݴϴ�.
	else
	{
		SectionName = FName(*(UsingSkill->ComboAttackName + TEXT("_") + FString::FromInt(ComboCount)));
	}



	// ������ �ִϸ��̼��� �ε��ϰ� �����մϴ�.
	PlayerCharacter->PlayAnimMontage(skillAnimMontage, 1.0f, SectionName);
	
	

}

void UPlayerAttack::ResetUsingSkill()
{
	// ������� ��ų�� ���ų�
	// ����������� ���罺ų���� �ٸ� ��ų�� �����Ϸ��� �ϰų�
	// �̹� �ִ뽺ų�޺��� ����� ��쿡�� �޺�ī��Ʈ�� 0���� �����մϴ�.
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
	// ������ ����� ���� �⺻���ݷ� ����
	float ATK = PlayerCharacter->GetPlayerAtk();

	// ���� ó�������� �߰� �������� ��������������
	float SemiDamage = 0.0f;
	float EndDamage = 0.0f;

	// �ߵ��� ��ų �޺� ����
	FSkillDamageInfo DamageInfo = UsingSkill->SkillDamage[ComboCount];

	// ���� ��ų �޺��� ������ ����
	FString DamageF = DamageInfo.DamageFormula;


	// FString���ε� �������� �����ϱ�
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
		// �Ǽ��� ������ ������ ��ĭ�� ��ġ������� �׳� �ѱ�� ����
		}
		else
		{
			SemiDamage *= 10.0f;
			SemiDamage += ((float)(DamageF[i])-48.0f);
		}
	}

	// ����� �������Ƿ� ���嵥������ �������ݴϴ�.
	if (SemiDamage > 0.0f)
	{
		EndDamage += SemiDamage;
		SemiDamage = 0.0f;
	}

	// ��ų ������ ���� ���ݽ��� ������ ���� ��������� �����մϴ�.
	FVector TraceStartVector = GetOwner()->GetActorLocation() + DamageInfo.SkillStartVector;
	FVector TraceEndVector = TraceStartVector + (DamageInfo.GetTraceDirection(GetOwner()) * DamageInfo.TraceDistance);

	// �˻�� ������ ��ü�� �����մϴ�.
	TArray<AActor*> IgnoreActor;
	IgnoreActor.Add(GetOwner());

	// Ʈ���̽� ����� ������ �迭
	TArray<FHitResult> hitResults;

	// ���� ���� �޺��� �´� �Ҹ��� �����Ǿ��ִٸ� ������ �Ҹ��� �׷��� �ʴٸ� ������ �޺��Ҹ��� ������ �����մϴ�. 
	int currentSoundNum = (UsingSkill->UseSoundWave.Num() > ComboCount) ? ComboCount : (UsingSkill->UseSoundWave.Num()-1);
	USoundWave* attackSound = Cast<USoundWave>(GetStreamManager()->LoadSynchronous(UsingSkill->UseSoundWave[currentSoundNum]));
	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), attackSound,GetOwner()->GetActorLocation());

	if (!UKismetSystemLibrary::SphereTraceMultiByProfile
	(GetWorld(), TraceStartVector, TraceEndVector, DamageInfo.TraceRange, TEXT("TryAttack"), true, IgnoreActor, EDrawDebugTrace::ForDuration, hitResults, true))
	{	
		for (int i = 0; i < hitResults.Num(); ++i)
		{
			//���ݿ� ���� ���� �ʾҴٸ� ���������ʽ��ϴ�.
			if(Cast<AEnemyCharacter>(hitResults[i].Actor) == nullptr) return;
			// �������Ͱ� ���̶�� �������� �����ϴ�.
			Cast<AEnemyCharacter>(hitResults[i].Actor)->GetDamaged(EndDamage);
			// ���� ��ġ�� ��ƼŬ�� �����մϴ�.
			Cast<ANormalLevelActor>(GetWorld()->GetLevelScriptActor())->SpawnParticle(Cast<UParticleSystem>(GetStreamManager()->LoadSynchronous(UsingSkill->HitParticle)), hitResults[i].ImpactPoint);

			// Ÿ����ġ�� �Ҹ��� ��������ݴϴ�.
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), Cast<USoundWave>(GetStreamManager()->LoadSynchronous(UsingSkill->HitSoundWave))
			, hitResults[i].ImpactPoint);
		}

	}


}

void UPlayerAttack::ResetAllSkill()
{	
	// ��� ���� ������ �ʱ�ȭ�մϴ�.
	ComboCount = 0;
	WaitSkill = nullptr;
	UsingSkill = nullptr;
}

