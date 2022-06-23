// Fill out your copyright notice in the Description page of Project Settings.


#include "MapChangeTrigger.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"

AMapChangeTrigger::AMapChangeTrigger()
{
	// ��Ż����Ʈ�� �ҷ��ɴϴ�.
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_PORTAL(TEXT("ParticleSystem'/Game/Resources/UsingParticle/PortalParticle/P_Portal.P_Portal'"));
	
	// Ʈ���� �ڽ��� �⺻������ �ʿ� �ƹ��� ǥ�õ� ���� �ʰ� �Ǿ��ֱ⶧���� ȭ�鿡 ���ϼ��ֵ��� ���� �����մϴ�.
	this->SetActorHiddenInGame(false);
	
	// ��Ʈ������Ʈ�� �ڽ�������Ʈ�� ������ ���̸� �ȵǹǷ� ���ü��� ���ݴϴ�.
	GetRootComponent()->SetVisibility(false);
	
	// ��Ż ��ƼŬ�� �����ϱ����� ��ƼŬ�ý���������Ʈ�� �����ϰ� �ٿ��ݴϴ�.
	PortalParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PARTICLE_SYSTEM_COMPONENT"));
	PortalParticle->SetupAttachment(GetRootComponent());
	
	if (P_PORTAL.Succeeded())
	{
		// �ҷ��� ��ƼŬ�� �����ϰ� ��������ݴϴ�.
		PortalParticle->SetTemplate(P_PORTAL.Object);
		PortalParticle->Activate(true);
	}
}

void AMapChangeTrigger::BeginPlay()
{
	Super::BeginPlay();

	// �÷��̾ ��Ż�� ������� �޼��尡 ȣ��ǰԲ� ���ε��մϴ�.
	OnActorBeginOverlap.AddDynamic(this, &AMapChangeTrigger::OnStartMapChange);

}

void AMapChangeTrigger::OnStartMapChange(AActor* OverlappedActor, AActor* OtherActor)
{
	// ���Ͱ� ��Ż�� ������ Ʈ���Źڽ��� ������ ���ο� ������ �Ѿ�ϴ�.
	GetHInstance()->OpenNewMap(NextMapName);
}
