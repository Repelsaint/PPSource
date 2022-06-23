// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "Animation/AnimInstance.h"

#include "EnemyInfo.generated.h"

/**
 * 
 */

 USTRUCT(BlueprintType)
struct HSUPP_API FEnemyInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int EnemyCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName EnemyName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Atk;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DropExp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int DropGold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath EnemyMeshPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UAnimInstance> EnemyAnimClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath AnimMontagePath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath AttackSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath HitParticle;

};
