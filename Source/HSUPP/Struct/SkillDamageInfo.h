// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSUPP.h"

#include "Enum/TraceToward.h"

#include "SkillDamageInfo.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct HSUPP_API FSkillDamageInfo
{

	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector SkillStartVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ETraceToward TraceDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TraceDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TraceRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString DamageFormula;

	FVector GetTraceDirection(AActor* ownerActor);
};
