// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ObjectPoolSystem/ObjectPoolSystem.h"
#include "Particles/ParticleSystemComponent.h"

#include "PoolableParticleSystem.generated.h"

UCLASS()
class HSUPP_API APoolableParticleSystem : public AActor, public IObjectPoolSystem
{
	GENERATED_BODY()


private:
	//파티클 시스템 컴포넌트입니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* ParticleSystem;
	
public:	
	// Sets default values for this actor's properties
	APoolableParticleSystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// 파티클이 끝났을때 호출될 메서드입니다.
	UFUNCTION()
	void ParticleFinished(UParticleSystemComponent* PSystem);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 파티클 시스템 컴포넌트를 불러낼때 사용합니다.
	FORCEINLINE UParticleSystemComponent* GetParticleSystem() const
	{ return ParticleSystem; }

	// 기본적인 풀러블 시스템에서 필수적으로 구현이 되야하는 부분을 매크로로 호출합니다.
	INIT_POOLSYSTEM

};
