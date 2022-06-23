// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ItemType.generated.h"
/**
 * 
 */

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Head = 0 ,
	Top = 1,
	Bottom = 2,
	Arm = 3,
	Shoes = 4,
	Potion= 5
};
