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
	//��ƼŬ �ý��� ������Ʈ�Դϴ�.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* ParticleSystem;
	
public:	
	// Sets default values for this actor's properties
	APoolableParticleSystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// ��ƼŬ�� �������� ȣ��� �޼����Դϴ�.
	UFUNCTION()
	void ParticleFinished(UParticleSystemComponent* PSystem);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ��ƼŬ �ý��� ������Ʈ�� �ҷ����� ����մϴ�.
	FORCEINLINE UParticleSystemComponent* GetParticleSystem() const
	{ return ParticleSystem; }

	// �⺻���� Ǯ���� �ý��ۿ��� �ʼ������� ������ �Ǿ��ϴ� �κ��� ��ũ�η� ȣ���մϴ�.
	INIT_POOLSYSTEM

};
