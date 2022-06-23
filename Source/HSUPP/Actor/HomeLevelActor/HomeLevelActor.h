// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSUPP.h"
#include "Actor/DefaultLevelActor/DefaultLevelActor.h"
#include "HomeLevelActor.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API AHomeLevelActor : public ADefaultLevelActor
{
	GENERATED_BODY()

private:
	// Ȩ ������ ����Ǿ����� ����� ������� �����Դϴ�.
	class USoundWave* HomeSoundWave;
public:
	AHomeLevelActor();

	virtual void BeginPlay() override;
	
};
