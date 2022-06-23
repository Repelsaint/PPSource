// Fill out your copyright notice in the Description page of Project Settings.


#include "HomeLevelActor.h"

AHomeLevelActor::AHomeLevelActor()
{
	//레벨의 타입을 홈으로 설정합니다.
	LevelType = ELevelActorType::Home;
	
	// 홈화면에서 틀어질 사운드를 호출하고 저장합니다.
	static ConstructorHelpers::FObjectFinder<USoundWave> SW_HL(TEXT("SoundWave'/Game/Resources/UsingSound/BackGround/Harp_of_Chronos.Harp_of_Chronos'"));
	if(SW_HL.Succeeded())
	 HomeSoundWave = SW_HL.Object;
}

void AHomeLevelActor::BeginPlay()
{

	Super::BeginPlay();
	
	// 홈액터가 실행이 되면 노래를 플레이시킵니다.
	UGameplayStatics::PlaySound2D(GetWorld(),HomeSoundWave);
}
