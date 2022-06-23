// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSUPP.h"
#include "Engine/LevelScriptActor.h"

#include "Enum/LevelActorType.h"
#include "DefaultLevelActor.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API ADefaultLevelActor : public ALevelScriptActor
{
	GENERATED_BODY()

protected:
	// �������� Ÿ���� ��Ÿ���� �����Դϴ�.
	ELevelActorType LevelType;
	

public:

	FORCEINLINE ELevelActorType GetLevelType() const
	{ return LevelType; }



};
