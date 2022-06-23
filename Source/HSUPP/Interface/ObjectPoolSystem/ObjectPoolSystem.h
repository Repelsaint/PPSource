// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Enum/PoolableObjectType.h"

#include "ObjectPoolSystem.generated.h"

#ifndef INIT_POOLSYSTEM
#define INIT_POOLSYSTEM	\
private: bool bCanRecyclable; \
public: \
 FORCEINLINE virtual void SetRecyclable(bool recyclable) override { bCanRecyclable = recyclable;}\
 FORCEINLINE virtual  bool GetRecyclable() const override { return bCanRecyclable;}
#endif

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UObjectPoolSystem : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HSUPP_API IObjectPoolSystem
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 인터페이스를 상속받는 객체가 반드시 생성해야하는 메서드구문입니다. 매크로로 생성하게 만들었습니다.
	virtual void SetRecyclable(bool recyclable) PURE_VIRTUAL(IObjectPoolSystem::SetCanRecyclable, );
	virtual bool GetRecyclable() const PURE_VIRTUAL(IObjectPoolSystem::GetRecyclable, return false;);


};
