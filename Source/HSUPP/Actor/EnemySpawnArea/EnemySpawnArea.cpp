// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawnArea.h"
#include "Actor/Character/EnemyCharacter/EnemyCharacter.h"
#include "Component/SpawnBox/SpawnBoxComponent.h"
#include "Actor/NormalLevelActor/NormalLevelActor.h"

// Sets default values
AEnemySpawnArea::AEnemySpawnArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// ���������� ���� �⺻���� ���������� ���ʹ� �����Դϴ�.
	EnemyCode = 1;
	SpawnEnemyCount = 10;

	// ������ ��ġ�� ������ �ڽ� ������Ʈ�� �����ϰ� ��Ʈ������Ʈ�� �����մϴ�.
	SpawnBoxComponent = CreateDefaultSubobject<USpawnBoxComponent>(TEXT("SPAWN_BOX_COMPONENT"));
	SetRootComponent(SpawnBoxComponent);


}

// Called when the game starts or when spawned
void AEnemySpawnArea::BeginPlay()
{
	Super::BeginPlay();
	// �������Ϳ� ������ ������ݴϴ�.
	Cast<ANormalLevelActor>(GetWorld()->GetLevelScriptActor())->AddSpawnArea(this);
	
}

// Called every frame
void AEnemySpawnArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AEnemySpawnArea::RespawnEnemy(class AEnemyCharacter* enemyCharacter)
{
	// ���� ���ȯ �ϴ� ������ ȣ���մϴ�.
	SpawnBoxComponent->ReSpawnEnemy(enemyCharacter);
}



