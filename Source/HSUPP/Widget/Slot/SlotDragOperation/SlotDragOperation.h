// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "SlotDragOperation.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API USlotDragOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	// 드래그를 시작한 슬롯입니다.
	class UBaseSlot* OriginSlot;
	
};
