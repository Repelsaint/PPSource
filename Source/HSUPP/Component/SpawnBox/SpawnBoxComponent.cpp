// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnBoxComponent.h"
#include "Actor/EnemySpawnArea/EnemySpawnArea.h"
#include "Actor/Character/EnemyCharacter/EnemyCharacter.h"


USpawnBoxComponent::USpawnBoxComponent()
{
	//기본 위치와 소환될칸의 범위를 초기화합니다.
	DefaultZeroVector = FVector::ZeroVector;
	SpawnAreaLength = 100.0f;

}


void USpawnBoxComponent::BeginPlay()
{
	Super::BeginPlay();

	// 제로벡터를 초기화합니다.
	InitDefaultZeroPosition();

	// 박스의 가로 세로 크기를 통해 가로 세로 최대 소환 가능 액터 수를 지정합니다.
	SpawnXCount = BoxExtent.X / SpawnAreaLength;
	SpawnYCount = BoxExtent.Y / SpawnAreaLength;
	
	// 액터의 소환할 몬스터의 종류와 수를 읽어옵니다.
	TTuple<int,int> areaEnemyInfo = Cast<AEnemySpawnArea>(GetOwner())->GetEnemyCodeNCount();

	// 소환해야할 수만큼 몬스터를 소환합니다.
	for (int i = 0; i < areaEnemyInfo.Get<1>(); ++i)
	{
		SpawnEnemy(areaEnemyInfo.Get<0>());
	}
}

void USpawnBoxComponent::InitDefaultZeroPosition()
{
	// 소환된 액터의 정 중앙값을 얻어냅니다.
	DefaultZeroVector = GetOwner()->GetActorLocation();

	// 가로 세로 길이의 반에 해당하는 값만큼 빼냅니다.
	DefaultZeroVector.X -= (BoxExtent.X / 2);
	DefaultZeroVector.Y -= (BoxExtent.Y / 2);
	
	//꼭지점에서 칸길이의 반만큼 추가한 값을 기본값으로 지정합니다.
	DefaultZeroVector.X += (SpawnAreaLength / 2);
	DefaultZeroVector.Y += (SpawnAreaLength / 2);

}

void USpawnBoxComponent::SpawnEnemy(int enemyCode)
{
	// 소환할 위치를 얻어냅니다.
	FVector spawnLocation = SetSpawnLocation();
	
	if (spawnLocation == FVector::ZeroVector)
	{
	//만약 제대로 얻지 못했다면 3초뒤에 다시 소환하게끔합니다.
		FTimerHandle timerHandle;
		FTimerDelegate timerDelegate;
		timerDelegate.BindUObject(this, &USpawnBoxComponent::SpawnEnemy, enemyCode);
		GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, 3.0f,false);
	}
	else
	{
		// 얻어낸 위치로 적을 소환합니다.
		AEnemyCharacter* newEnemy =	GetWorld()->SpawnActor<AEnemyCharacter>(spawnLocation,FRotator::ZeroRotator);

		// 생성된 적의 코드를 지정하고 초기화해줍니다.
		newEnemy->SetEnemyCode(enemyCode);
		newEnemy->InitializeEnemyCharacter();
	}
				
		
	


}

FVector USpawnBoxComponent::SetSpawnLocation()
{
	// 구간내 소환될 랜덤한 숫자를 뽑습니다
	int randNum = FMath::RandRange(0, (SpawnYCount * SpawnXCount - 1));

	// 숫자를 토대로 x,y 위치를 지정합니다.
	int spawnXNum = randNum / SpawnXCount;
	int spawnYNum = randNum % SpawnXCount;

	// 뽑은 위치를 벡터값으로 변환합니다.
	FVector spawnVector = DefaultZeroVector;
	spawnVector.X += (SpawnAreaLength * spawnXNum);
	spawnVector.Y += (SpawnAreaLength * spawnYNum);


	TArray<AActor*> ignoreActors;
	FHitResult hitResults;

	// 더이상 소환할 자리가 없으면 멈추기 위한 최대 카운트수 와 현재 반복횟수
	int maxCount = 100;
	int currentCount = 0;
	
	for (int i = 0; i < maxCount; ++i)
	{
		// 지정한 위치에 적이 있다면 없을때까지 랜덤숫자를 다시 뽑습니다.
		if (UKismetSystemLibrary::LineTraceSingleByProfile
		(GetWorld(), (spawnVector + (FVector::UpVector * 200.0f)), spawnVector, TEXT("SpawnEnemy"), true, ignoreActors, EDrawDebugTrace::ForDuration, hitResults, true))
		{
			++currentCount;
		}
		else
		{
			return spawnVector;
		}
	}

	// 100회의 리셋기회 동안 스폰위치를 찾지 못했다면 제로벡터를 리턴합니다.
	return FVector::ZeroVector;
}


void USpawnBoxComponent::ReSpawnEnemy(AEnemyCharacter* enemyCharacter)
{
	// 리스폰할 위치를 얻어옵니다.
	FVector spawnLocation = SetSpawnLocation();
	// 제로백터라면 3초뒤에 리스폰을 다시시도합니다.
	if (spawnLocation == FVector::ZeroVector)
	{
		FTimerHandle timerHandle;
		FTimerDelegate timerDelegate;

		timerDelegate.BindUObject(this, &USpawnBoxComponent::ReSpawnEnemy, enemyCharacter);

		GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, 3.0f, false);

	}
	else
	{
		//찾아낸 위치로 적을 이동시키고 초기화합니다.
		enemyCharacter->SetActorLocation(spawnLocation);
		enemyCharacter->InitializeEnemyCharacter();
	}

}
