// Fill out your copyright notice in the Description page of Project Settings.


#include "MapChangeTrigger.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"

AMapChangeTrigger::AMapChangeTrigger()
{
	// 포탈이펙트를 불러옵니다.
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_PORTAL(TEXT("ParticleSystem'/Game/Resources/UsingParticle/PortalParticle/P_Portal.P_Portal'"));
	
	// 트리거 박스는 기본적으로 맵에 아무런 표시도 되지 않게 되어있기때문에 화면에 보일수있도록 값을 조정합니다.
	this->SetActorHiddenInGame(false);
	
	// 루트컴포넌트인 박스컴포넌트의 외형은 보이면 안되므로 가시성을 꺼줍니다.
	GetRootComponent()->SetVisibility(false);
	
	// 포탈 파티클을 생성하기위해 파티클시스템컴포넌트를 생성하고 붙여줍니다.
	PortalParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PARTICLE_SYSTEM_COMPONENT"));
	PortalParticle->SetupAttachment(GetRootComponent());
	
	if (P_PORTAL.Succeeded())
	{
		// 불러온 파티클을 지정하고 실행시켜줍니다.
		PortalParticle->SetTemplate(P_PORTAL.Object);
		PortalParticle->Activate(true);
	}
}

void AMapChangeTrigger::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어가 포탈에 닿았을때 메서드가 호출되게끔 바인딩합니다.
	OnActorBeginOverlap.AddDynamic(this, &AMapChangeTrigger::OnStartMapChange);

}

void AMapChangeTrigger::OnStartMapChange(AActor* OverlappedActor, AActor* OtherActor)
{
	// 액터가 포탈에 닿으면 트리거박스에 지정된 새로운 맵으로 넘어갑니다.
	GetHInstance()->OpenNewMap(NextMapName);
}
