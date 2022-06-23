// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"


// Sets default values
ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �⺻ �������� �������� �����صӴϴ�.
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
	// ����ü���� �������� ������ŭ ���ҽ�ŵ�ϴ�.
	CurrentHP -= damaged;

	// ����ü���� �������� �ް� 0 ���Ϸ� �������ٸ�
	if (CurrentHP <= 0)
	{
		//����ü���� 0���� �����մϴ�.
		CurrentHP = 0;
		// ���̻� ������ �����ʵ��� �ݸ����� ����ΰ� ����޼��带 ȣ���մϴ�.
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
		CharacterDie();
	}
}

void ABaseCharacter::CharacterDie()
{
}

