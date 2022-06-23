// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolableParticleSystem.h"

// Sets default values
APoolableParticleSystem::APoolableParticleSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ��ƼŬ �ý���������Ʈ�� �����ϰ� ��Ʈ������Ʈ�� �����մϴ�.
	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PARTICLE_SYSTEM"));
	SetRootComponent(ParticleSystem);
}

// Called when the game starts or when spawned
void APoolableParticleSystem::BeginPlay()
{
	Super::BeginPlay();
	
	// ��ƼŬ�� ������ �޼��尡 ȣ���� �ǵ��� ���ε��մϴ�.
	ParticleSystem->OnSystemFinished.AddDynamic(this, &APoolableParticleSystem::ParticleFinished);
}

void APoolableParticleSystem::ParticleFinished(UParticleSystemComponent* PSystem)
{
	// ��ƼŬ�� ����� ������ ��ƼŬ ���밡�� ������ ���ݴϴ�.
	SetRecyclable(true);
}

// Called every frame
void APoolableParticleSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

