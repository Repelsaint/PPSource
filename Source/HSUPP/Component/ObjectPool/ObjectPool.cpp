// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPool.h"


IObjectPoolSystem* UObjectPool::GetRecyclableObject() const
{
	// 배열에 저장된 객체가 없다면 Nullptr을 반환합니다.
	if(PoolableObjects.Num() == 0) return nullptr;
	   
	   
	for (auto poolobject : PoolableObjects)
	{
		// 재사용가능 객체가 있다면 재사용하면서 재사용가능값을 false로 지정해줍니다.
		if (poolobject->GetRecyclable())
		{
			poolobject->SetRecyclable(false);

			return poolobject;
		}
	}

	// 재사용가능한 객체가 없다면 Nullptr을 반환합니다.
		return nullptr;
}