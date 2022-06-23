// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnBoxComponent.h"
#include "Actor/EnemySpawnArea/EnemySpawnArea.h"
#include "Actor/Character/EnemyCharacter/EnemyCharacter.h"


USpawnBoxComponent::USpawnBoxComponent()
{
	//�⺻ ��ġ�� ��ȯ��ĭ�� ������ �ʱ�ȭ�մϴ�.
	DefaultZeroVector = FVector::ZeroVector;
	SpawnAreaLength = 100.0f;

}


void USpawnBoxComponent::BeginPlay()
{
	Super::BeginPlay();

	// ���κ��͸� �ʱ�ȭ�մϴ�.
	InitDefaultZeroPosition();

	// �ڽ��� ���� ���� ũ�⸦ ���� ���� ���� �ִ� ��ȯ ���� ���� ���� �����մϴ�.
	SpawnXCount = BoxExtent.X / SpawnAreaLength;
	SpawnYCount = BoxExtent.Y / SpawnAreaLength;
	
	// ������ ��ȯ�� ������ ������ ���� �о�ɴϴ�.
	TTuple<int,int> areaEnemyInfo = Cast<AEnemySpawnArea>(GetOwner())->GetEnemyCodeNCount();

	// ��ȯ�ؾ��� ����ŭ ���͸� ��ȯ�մϴ�.
	for (int i = 0; i < areaEnemyInfo.Get<1>(); ++i)
	{
		SpawnEnemy(areaEnemyInfo.Get<0>());
	}
}

void USpawnBoxComponent::InitDefaultZeroPosition()
{
	// ��ȯ�� ������ �� �߾Ӱ��� �����ϴ�.
	DefaultZeroVector = GetOwner()->GetActorLocation();

	// ���� ���� ������ �ݿ� �ش��ϴ� ����ŭ �����ϴ�.
	DefaultZeroVector.X -= (BoxExtent.X / 2);
	DefaultZeroVector.Y -= (BoxExtent.Y / 2);
	
	//���������� ĭ������ �ݸ�ŭ �߰��� ���� �⺻������ �����մϴ�.
	DefaultZeroVector.X += (SpawnAreaLength / 2);
	DefaultZeroVector.Y += (SpawnAreaLength / 2);

}

void USpawnBoxComponent::SpawnEnemy(int enemyCode)
{
	// ��ȯ�� ��ġ�� �����ϴ�.
	FVector spawnLocation = SetSpawnLocation();
	
	if (spawnLocation == FVector::ZeroVector)
	{
	//���� ����� ���� ���ߴٸ� 3�ʵڿ� �ٽ� ��ȯ�ϰԲ��մϴ�.
		FTimerHandle timerHandle;
		FTimerDelegate timerDelegate;
		timerDelegate.BindUObject(this, &USpawnBoxComponent::SpawnEnemy, enemyCode);
		GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, 3.0f,false);
	}
	else
	{
		// �� ��ġ�� ���� ��ȯ�մϴ�.
		AEnemyCharacter* newEnemy =	GetWorld()->SpawnActor<AEnemyCharacter>(spawnLocation,FRotator::ZeroRotator);

		// ������ ���� �ڵ带 �����ϰ� �ʱ�ȭ���ݴϴ�.
		newEnemy->SetEnemyCode(enemyCode);
		newEnemy->InitializeEnemyCharacter();
	}
				
		
	


}

FVector USpawnBoxComponent::SetSpawnLocation()
{
	// ������ ��ȯ�� ������ ���ڸ� �̽��ϴ�
	int randNum = FMath::RandRange(0, (SpawnYCount * SpawnXCount - 1));

	// ���ڸ� ���� x,y ��ġ�� �����մϴ�.
	int spawnXNum = randNum / SpawnXCount;
	int spawnYNum = randNum % SpawnXCount;

	// ���� ��ġ�� ���Ͱ����� ��ȯ�մϴ�.
	FVector spawnVector = DefaultZeroVector;
	spawnVector.X += (SpawnAreaLength * spawnXNum);
	spawnVector.Y += (SpawnAreaLength * spawnYNum);


	TArray<AActor*> ignoreActors;
	FHitResult hitResults;

	// ���̻� ��ȯ�� �ڸ��� ������ ���߱� ���� �ִ� ī��Ʈ�� �� ���� �ݺ�Ƚ��
	int maxCount = 100;
	int currentCount = 0;
	
	for (int i = 0; i < maxCount; ++i)
	{
		// ������ ��ġ�� ���� �ִٸ� ���������� �������ڸ� �ٽ� �̽��ϴ�.
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

	// 100ȸ�� ���±�ȸ ���� ������ġ�� ã�� ���ߴٸ� ���κ��͸� �����մϴ�.
	return FVector::ZeroVector;
}


void USpawnBoxComponent::ReSpawnEnemy(AEnemyCharacter* enemyCharacter)
{
	// �������� ��ġ�� ���ɴϴ�.
	FVector spawnLocation = SetSpawnLocation();
	// ���ι��Ͷ�� 3�ʵڿ� �������� �ٽýõ��մϴ�.
	if (spawnLocation == FVector::ZeroVector)
	{
		FTimerHandle timerHandle;
		FTimerDelegate timerDelegate;

		timerDelegate.BindUObject(this, &USpawnBoxComponent::ReSpawnEnemy, enemyCharacter);

		GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, 3.0f, false);

	}
	else
	{
		//ã�Ƴ� ��ġ�� ���� �̵���Ű�� �ʱ�ȭ�մϴ�.
		enemyCharacter->SetActorLocation(spawnLocation);
		enemyCharacter->InitializeEnemyCharacter();
	}

}
