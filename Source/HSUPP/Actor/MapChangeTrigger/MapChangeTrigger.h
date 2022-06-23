// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSUPP.h"
#include "Engine/TriggerBox.h"
#include "MapChangeTrigger.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API AMapChangeTrigger : public ATriggerBox
{
	GENERATED_BODY()
private:
	// ��Ż�� ���� �̵��� ���� �̸��Դϴ�.
	UPROPERTY(EditAnywhere)
		FName NextMapName;

	// ��Ż�� ����� ������ ��ƼŬ�Դϴ�.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UParticleSystemComponent* PortalParticle;

public:
	AMapChangeTrigger();
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnStartMapChange(AActor* OverlappedActor, AActor* OtherActor);
	
};
