// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSUPP.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnArea.generated.h"

UCLASS()
class HSUPP_API AEnemySpawnArea : public AActor
{
	GENERATED_BODY()
	
private:
	// 소환할 적의 종류를 결정하는 고유코드입니다.
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int EnemyCode;
	
	// 소환할 개체수를 결정하는 변수입니다.
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int SpawnEnemyCount;

	// 소환할 범위를 지정하고 소환을 시켜줄 컴포넌트입니다.
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USpawnBoxComponent* SpawnBoxComponent;


public:	
	// Sets default values for this actor's properties
	AEnemySpawnArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 적의 코드와 수를 한번에 불러오는 메서드입니다.
	FORCEINLINE TTuple<int,int> GetEnemyCodeNCount()
	{ return MakeTuple(EnemyCode,SpawnEnemyCount); }

	// 적의 코드만을 얻을때 사용되는 메서드입니다.
	FORCEINLINE int GetAreaEnemyCode() const
	{ return EnemyCode;}

	void RespawnEnemy(class AEnemyCharacter* enemyCharacter);

	

};
