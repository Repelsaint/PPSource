// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSUPP.h"
#include "Components/BoxComponent.h"
#include "SpawnBoxComponent.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API USpawnBoxComponent : public UBoxComponent
{
	GENERATED_BODY()

private:
	
	// 스폰기준 맨 앞에 위치한 벡터입니다.
	FVector DefaultZeroVector;

	// 가로 세로로 총 몇개의 구획으로 나눌지 나타냅니다.
	int SpawnXCount;
	int SpawnYCount;

	// 각 구획의 길이를 나타냅니다.
	float SpawnAreaLength;


private:
	//소환 위치를 지정합니다.
	FVector SetSpawnLocation();



public:
	USpawnBoxComponent();

	virtual void BeginPlay() override;


	// 스폰될 위치의 첫번째 자리를 초기화합니다.
	void InitDefaultZeroPosition();

	// 적을 소환할때 사용되는 메서드입니다.
	void SpawnEnemy(int enemyCode);

	// 적 사망후 위치 재조정을 위한 메서드입니다.
	void ReSpawnEnemy(class AEnemyCharacter* enemyCharacter);
};
