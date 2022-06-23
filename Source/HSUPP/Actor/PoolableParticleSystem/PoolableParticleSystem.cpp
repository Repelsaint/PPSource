// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolableParticleSystem.h"

// Sets default values
APoolableParticleSystem::APoolableParticleSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 파티클 시스템컴포넌트를 생성하고 루트컴포넌트로 지정합니다.
	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PARTICLE_SYSTEM"));
	SetRootComponent(ParticleSystem);
}

// Called when the game starts or when spawned
void APoolableParticleSystem::BeginPlay()
{
	Super::BeginPlay();
	
	// 파티클이 끝나면 메서드가 호출이 되도록 바인딩합니다.
	ParticleSystem->OnSystemFinished.AddDynamic(this, &APoolableParticleSystem::ParticleFinished);
}

void APoolableParticleSystem::ParticleFinished(UParticleSystemComponent* PSystem)
{
	// 파티클의 재생이 끝나면 파티클 재사용가능 설정을 해줍니다.
	SetRecyclable(true);
}

// Called every frame
void APoolableParticleSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

