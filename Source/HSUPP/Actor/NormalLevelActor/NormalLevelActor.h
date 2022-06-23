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
	//스폰 지역액터를 가지고 있을 변수입니다.
	TArray<class AEnemySpawnArea*> SpawnAreas;

	// 재사용가능한 객체를 관리하는 컴포넌트입니다.
	UPROPERTY()
		UObjectPool* ObjectPoolComponent;

		//뒤에서 실행되고 있을 배경음악 컴포넌트입니다.
	UAudioComponent* LevelBackgroundMusic;

	USoundWave* DungeonSound;
	USoundWave* VillageSound;


protected:
	ANormalLevelActor();
	virtual void BeginPlay() override;

public:
	// 배열에 스폰 지역을 추가하는 메서드입니다.
	void AddSpawnArea(class AEnemySpawnArea* spawnArea);

	// 코드에 맞는 스폰지역을 찾을때 호출합니다.
	class AEnemySpawnArea* GetSpawnAreaArray(int enemyCode);

	// 파티클 생성시 사용하는 메서드입니다.
	class APoolableParticleSystem* SpawnParticle(UParticleSystem* particleSystem, FVector spawnLocation, FRotator spawnRotation = FRotator::ZeroRotator);

	// 오브젝트 풀 시스템을 찾을때 사용합니다.
	FORCEINLINE UObjectPool* GetObjectPoolComponent() const
	{
		return ObjectPoolComponent;
	}

	// 음악 크기를 조정할때 사용합니다.
	void SetBGMMusicSound(float soundSize);
};
