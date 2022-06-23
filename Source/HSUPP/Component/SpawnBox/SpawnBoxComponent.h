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
	
	// �������� �� �տ� ��ġ�� �����Դϴ�.
	FVector DefaultZeroVector;

	// ���� ���η� �� ��� ��ȹ���� ������ ��Ÿ���ϴ�.
	int SpawnXCount;
	int SpawnYCount;

	// �� ��ȹ�� ���̸� ��Ÿ���ϴ�.
	float SpawnAreaLength;


private:
	//��ȯ ��ġ�� �����մϴ�.
	FVector SetSpawnLocation();



public:
	USpawnBoxComponent();

	virtual void BeginPlay() override;


	// ������ ��ġ�� ù��° �ڸ��� �ʱ�ȭ�մϴ�.
	void InitDefaultZeroPosition();

	// ���� ��ȯ�Ҷ� ���Ǵ� �޼����Դϴ�.
	void SpawnEnemy(int enemyCode);

	// �� ����� ��ġ �������� ���� �޼����Դϴ�.
	void ReSpawnEnemy(class AEnemyCharacter* enemyCharacter);
};
