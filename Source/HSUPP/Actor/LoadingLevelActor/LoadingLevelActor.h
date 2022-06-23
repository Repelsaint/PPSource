// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSUPP.h"
#include "Actor/DefaultLevelActor/DefaultLevelActor.h"
#include "LoadingLevelActor.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API ALoadingLevelActor : public ADefaultLevelActor
{
	GENERATED_BODY()
	
public:
	ALoadingLevelActor();
	virtual void BeginPlay() override;



};
