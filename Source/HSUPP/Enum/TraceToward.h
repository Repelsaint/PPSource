// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "TraceToward.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ETraceToward : uint8
{
	FORWARD		UMETA(DisplayName = Forward),
	BACK		UMETA(DisplayName = Back),
	RIGHT		UMETA(DisplayName = Right),
	LEFT		UMETA(DisplayName = Left)
};
