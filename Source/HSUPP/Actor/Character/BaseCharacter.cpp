// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"


// Sets default values
ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 기본 팀설정을 없음으로 지정해둡니다.
	Team = FGenericTeamId::NoTeam;

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseCharacter::GetDamaged(int damaged)
{
	// 현재체력을 데미지를 받은만큼 감소시킵니다.
	CurrentHP -= damaged;

	// 현재체력이 데미지를 받고 0 이하로 떨어졌다면
	if (CurrentHP <= 0)
	{
		//현재체력을 0으로 지정합니다.
		CurrentHP = 0;
		// 더이상 데미지 받지않도록 콜리전을 비워두고 사망메서드를 호출합니다.
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
		CharacterDie();
	}
}

void ABaseCharacter::CharacterDie()
{
}

