// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingLevelActor.h"

ALoadingLevelActor::ALoadingLevelActor()
{
	// 레벨의 타입을 로딩으로 지정합니다.
	LevelType = ELevelActorType::Loading;
}

void ALoadingLevelActor::BeginPlay()
{
	
	// 로딩창이 뜬후 일정 시간뒤에 다음 맵으로 넘어가게끔 지정합니다.
	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate;
	timerDelegate.BindUObject(GetHInstance(), &UHInstance::OpenNextMap);


	GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate,0.5f,false);
	
}