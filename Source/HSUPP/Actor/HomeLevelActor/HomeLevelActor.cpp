// Fill out your copyright notice in the Description page of Project Settings.


#include "HomeLevelActor.h"

AHomeLevelActor::AHomeLevelActor()
{
	//������ Ÿ���� Ȩ���� �����մϴ�.
	LevelType = ELevelActorType::Home;
	
	// Ȩȭ�鿡�� Ʋ���� ���带 ȣ���ϰ� �����մϴ�.
	static ConstructorHelpers::FObjectFinder<USoundWave> SW_HL(TEXT("SoundWave'/Game/Resources/UsingSound/BackGround/Harp_of_Chronos.Harp_of_Chronos'"));
	if(SW_HL.Succeeded())
	 HomeSoundWave = SW_HL.Object;
}

void AHomeLevelActor::BeginPlay()
{

	Super::BeginPlay();
	
	// Ȩ���Ͱ� ������ �Ǹ� �뷡�� �÷��̽�ŵ�ϴ�.
	UGameplayStatics::PlaySound2D(GetWorld(),HomeSoundWave);
}
