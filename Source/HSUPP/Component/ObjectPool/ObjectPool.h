// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Interface/ObjectPoolSystem/ObjectPoolSystem.h"
#include "ObjectPool.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API UObjectPool : public UObject
{
	GENERATED_BODY()

private:
	//������ ��ü�� ����� �迭�Դϴ�.
	TArray<IObjectPoolSystem*> PoolableObjects;


public:
	// ������ ��ü�� �迭�� �����ϰ� �����ݴϴ�.
	template<typename T>
	FORCEINLINE T* RegisterPoolableObject(T* poolableObject)
	{
		
		PoolableObjects.Add(poolableObject);
		return poolableObject;
	}

	//���� ���밴ü�� ���ϴ� ���·� ĳ�����Ͽ� ��ȯ�մϴ�.
	template<typename T>
	FORCEINLINE T* GetRecyclableObject() const
	{
		return Cast<T>(GetRecyclableObject());
	}

	// �迭���� ���� ������ ��ü�� ��ȯ�մϴ�.
	IObjectPoolSystem* GetRecyclableObject() const;

	
};
