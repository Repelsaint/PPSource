// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingLevelActor.h"

ALoadingLevelActor::ALoadingLevelActor()
{
	// ������ Ÿ���� �ε����� �����մϴ�.
	LevelType = ELevelActorType::Loading;
}

void ALoadingLevelActor::BeginPlay()
{
	
	// �ε�â�� ���� ���� �ð��ڿ� ���� ������ �Ѿ�Բ� �����մϴ�.
	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate;
	timerDelegate.BindUObject(GetHInstance(), &UHInstance::OpenNextMap);


	GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate,0.5f,false);
	
}