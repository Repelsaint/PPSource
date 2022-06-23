// Fill out your copyright notice in the Description page of Project Settings.


#include "HGameMode.h"
#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"
#include "Actor/Controller/CharacterController.h"
#include "Actor/DefaultLevelActor/DefaultLevelActor.h"


AHGameMode::AHGameMode()
{
	// 기본 컨트롤러를 지정합니다.
		PlayerControllerClass = ACharacterController::StaticClass();
	
}
