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
	// 포탈에 의해 이동할 맵의 이름입니다.
	UPROPERTY(EditAnywhere)
		FName NextMapName;

	// 포탈의 모양을 지정할 파티클입니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UParticleSystemComponent* PortalParticle;

public:
	AMapChangeTrigger();
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnStartMapChange(AActor* OverlappedActor, AActor* OtherActor);
	
};
