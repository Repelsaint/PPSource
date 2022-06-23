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

	// 에러방지를 위한 기본으로 가지고있을 에너미 정보입니다.
	EnemyCode = 1;
	SpawnEnemyCount = 10;

	// 스폰할 위치를 지정할 박스 컴포넌트를 생성하고 루트컴포넌트로 지정합니다.
	SpawnBoxComponent = CreateDefaultSubobject<USpawnBoxComponent>(TEXT("SPAWN_BOX_COMPONENT"));
	SetRootComponent(SpawnBoxComponent);


}

// Called when the game starts or when spawned
void AEnemySpawnArea::BeginPlay()
{
	Super::BeginPlay();
	// 레벨액터에 본인을 등록해줍니다.
	Cast<ANormalLevelActor>(GetWorld()->GetLevelScriptActor())->AddSpawnArea(this);
	
}

// Called every frame
void AEnemySpawnArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AEnemySpawnArea::RespawnEnemy(class AEnemyCharacter* enemyCharacter)
{
	// 적을 재소환 하는 구문을 호출합니다.
	SpawnBoxComponent->ReSpawnEnemy(enemyCharacter);
}



