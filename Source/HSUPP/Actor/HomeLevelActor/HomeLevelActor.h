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
	// 홈 레벨이 실행되었을때 실행될 배경음의 종류입니다.
	class USoundWave* HomeSoundWave;
public:
	AHomeLevelActor();

	virtual void BeginPlay() override;
	
};
