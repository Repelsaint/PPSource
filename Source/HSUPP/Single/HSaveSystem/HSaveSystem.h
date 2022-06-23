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
	// 저장 혹은 로드시에 사용될 슬롯이름과 인덱스입니다.
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	FString SaveSlotName;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	int32 SaveIndex;

	// 저장할 정보입니다.
	UPROPERTY()
		FPlayerInfo PlayerInfo;
};
