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
	// ���� �ε忡 ���� �̸��� �ε����Դϴ�.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FString SaveSlotName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int32 SaveIndex;

		// ������ ������ �Ǿ��� �������Դϴ�.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TArray<bool> MuteStates;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TArray<float> SoundsValues;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FIntPoint ScreenResolution;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FString WindowMode;
};
