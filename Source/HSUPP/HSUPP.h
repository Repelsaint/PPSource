// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"

#include "GameInstance/HInstance.h"


#ifndef UEMACRO
#define UEMACRO

#define L(_log_, ...) UE_LOG(LogTemp, Warning, _log_, ##__VA_ARGS__)
#define SL(_log_)		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, _log_)
#define GetStreamManager() Cast<UHInstance>(GetWorld()->GetGameInstance())->GetStreamableManager()
#define GetHInstance() Cast<UHInstance>(GetGameInstance())
#define SaveOption(newWindowMode,newResolution) Cast<UHInstance>(GetGameInstance())->OptionSave(newWindowMode,newResolution)

#endif
