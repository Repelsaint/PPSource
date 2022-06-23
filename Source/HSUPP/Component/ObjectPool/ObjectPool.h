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
	//생성된 객체가 저장될 배열입니다.
	TArray<IObjectPoolSystem*> PoolableObjects;


public:
	// 생성된 객체를 배열에 저장하고 돌려줍니다.
	template<typename T>
	FORCEINLINE T* RegisterPoolableObject(T* poolableObject)
	{
		
		PoolableObjects.Add(poolableObject);
		return poolableObject;
	}

	//얻어온 재사용객체를 원하는 형태로 캐스팅하여 반환합니다.
	template<typename T>
	FORCEINLINE T* GetRecyclableObject() const
	{
		return Cast<T>(GetRecyclableObject());
	}

	// 배열에서 재사용 가능한 객체를 반환합니다.
	IObjectPoolSystem* GetRecyclableObject() const;

	
};
