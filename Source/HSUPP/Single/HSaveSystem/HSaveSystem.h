// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#include "Struct/PlayerInfo.h"

#include "HSaveSystem.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API UHSaveSystem : public USaveGame
{
	GENERATED_BODY()
	
public:
	// ���� Ȥ�� �ε�ÿ� ���� �����̸��� �ε����Դϴ�.
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	FString SaveSlotName;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	int32 SaveIndex;

	// ������ �����Դϴ�.
	UPROPERTY()
		FPlayerInfo PlayerInfo;
};
