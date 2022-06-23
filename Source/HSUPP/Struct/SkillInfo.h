// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSUPP.h"
#include "Struct/SkillDamageInfo.h"
#include "Engine/DataTable.h"


#include "SkillInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct HSUPP_API FSkillInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SkillCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ComboAttackName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath SkillIconPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath AnimMontagePath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FSkillDamageInfo> SkillDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath HitParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FSoftObjectPath> UseSoundWave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSoftObjectPath HitSoundWave;

};
