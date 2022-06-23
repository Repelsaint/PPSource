// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/DefaultLevelActor/DefaultLevelActor.h"
#include "Component/ObjectPool/ObjectPool.h"
#include "NormalLevelActor.generated.h"

/**
 *
 */
UCLASS()
class HSUPP_API ANormalLevelActor : public ADefaultLevelActor
{
	GENERATED_BODY()

private:
	//���� �������͸� ������ ���� �����Դϴ�.
	TArray<class AEnemySpawnArea*> SpawnAreas;

	// ���밡���� ��ü�� �����ϴ� ������Ʈ�Դϴ�.
	UPROPERTY()
		UObjectPool* ObjectPoolComponent;

		//�ڿ��� ����ǰ� ���� ������� ������Ʈ�Դϴ�.
	UAudioComponent* LevelBackgroundMusic;

	USoundWave* DungeonSound;
	USoundWave* VillageSound;


protected:
	ANormalLevelActor();
	virtual void BeginPlay() override;

public:
	// �迭�� ���� ������ �߰��ϴ� �޼����Դϴ�.
	void AddSpawnArea(class AEnemySpawnArea* spawnArea);

	// �ڵ忡 �´� ���������� ã���� ȣ���մϴ�.
	class AEnemySpawnArea* GetSpawnAreaArray(int enemyCode);

	// ��ƼŬ ������ ����ϴ� �޼����Դϴ�.
	class APoolableParticleSystem* SpawnParticle(UParticleSystem* particleSystem, FVector spawnLocation, FRotator spawnRotation = FRotator::ZeroRotator);

	// ������Ʈ Ǯ �ý����� ã���� ����մϴ�.
	FORCEINLINE UObjectPool* GetObjectPoolComponent() const
	{
		return ObjectPoolComponent;
	}

	// ���� ũ�⸦ �����Ҷ� ����մϴ�.
	void SetBGMMusicSound(float soundSize);
};
