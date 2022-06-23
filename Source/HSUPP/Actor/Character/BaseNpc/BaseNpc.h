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
	//NPC ���� �ڵ�
	UPROPERTY(EditAnywhere)
	int NpcCode;

	// NPC�� �̸�
	UPROPERTY(EditAnywhere)
	FText NpcName;

	//NPC�� ��ȣ�ۿ��� ������ ����
	UPROPERTY(EditAnywhere)
	class UInteractableAreaComponent* InteractableArea;

	//NPC ��ȭâ �������Ʈ
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
