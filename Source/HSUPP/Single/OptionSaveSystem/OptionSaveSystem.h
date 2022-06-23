// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OptionSaveSystem.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API UOptionSaveSystem : public USaveGame
{
	GENERATED_BODY()

public:
	// 저장 로드에 사용될 이름과 인덱스입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FString SaveSlotName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int32 SaveIndex;

		// 실제로 저장이 되어질 정보들입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TArray<bool> MuteStates;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TArray<float> SoundsValues;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FIntPoint ScreenResolution;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FString WindowMode;
};
