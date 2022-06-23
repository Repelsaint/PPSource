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
	// ��ȯ�� ���� ������ �����ϴ� �����ڵ��Դϴ�.
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int EnemyCode;
	
	// ��ȯ�� ��ü���� �����ϴ� �����Դϴ�.
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int SpawnEnemyCount;

	// ��ȯ�� ������ �����ϰ� ��ȯ�� ������ ������Ʈ�Դϴ�.
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

	// ���� �ڵ�� ���� �ѹ��� �ҷ����� �޼����Դϴ�.
	FORCEINLINE TTuple<int,int> GetEnemyCodeNCount()
	{ return MakeTuple(EnemyCode,SpawnEnemyCount); }

	// ���� �ڵ常�� ������ ���Ǵ� �޼����Դϴ�.
	FORCEINLINE int GetAreaEnemyCode() const
	{ return EnemyCode;}

	void RespawnEnemy(class AEnemyCharacter* enemyCharacter);

	

};
