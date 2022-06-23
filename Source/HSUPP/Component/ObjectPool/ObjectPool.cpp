// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPool.h"


IObjectPoolSystem* UObjectPool::GetRecyclableObject() const
{
	// �迭�� ����� ��ü�� ���ٸ� Nullptr�� ��ȯ�մϴ�.
	if(PoolableObjects.Num() == 0) return nullptr;
	   
	   
	for (auto poolobject : PoolableObjects)
	{
		// ���밡�� ��ü�� �ִٸ� �����ϸ鼭 ���밡�ɰ��� false�� �������ݴϴ�.
		if (poolobject->GetRecyclable())
		{
			poolobject->SetRecyclable(false);

			return poolobject;
		}
	}

	// ���밡���� ��ü�� ���ٸ� Nullptr�� ��ȯ�մϴ�.
		return nullptr;
}