// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSUPP.h"
#include "GameFramework/Character.h"

#include "BaseNpc.generated.h"

UCLASS()
class HSUPP_API ABaseNpc : public ACharacter
{
	GENERATED_BODY()


private:
	//NPC 고유 코드
	UPROPERTY(EditAnywhere)
	int NpcCode;

	// NPC의 이름
	UPROPERTY(EditAnywhere)
	FText NpcName;

	//NPC와 상호작용이 가능한 영역
	UPROPERTY(EditAnywhere)
	class UInteractableAreaComponent* InteractableArea;

	//NPC 대화창 블루프린트
	TSubclassOf<class UNpcDialog> BP_NpcDialog;

public:
	// Sets default values for this character's properties
	ABaseNpc();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
