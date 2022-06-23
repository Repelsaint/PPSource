// Fill out your copyright notice in the Description page of Project Settings.


#include "HInstance.h"
#include "Engine/Engine.h"

#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"
#include "Actor/Controller/CharacterController.h"


#include "Widget/MainWidget.h"
#include "Widget/HomeWidget/HomeWidget.h"

#include "GameMode/HGameMode.h"

#include "Single/GameSettings/GameSettings.h"
#include "Single/HSaveSystem/HSaveSystem.h"
#include "Single/OptionSaveSystem/OptionSaveSystem.h"

#include "JsonObjectConverter.h"

#include "Serialization/JsonWriter.h"

UHInstance::UHInstance()
{
	// 음량 클래스를 불러옵니다.
	static ConstructorHelpers::FObjectFinder<USoundClass> BP_MASTER_SOUND_CLASS(TEXT("SoundClass'/Game/Single/BP_MaterSoundClass.BP_MaterSoundClass'"));
	
	// 게임세팅을 불러옵니다.
	GameSetting = CreateDefaultSubobject<UGameSettings>(TEXT("GAME_SETTING"));
	if (BP_MASTER_SOUND_CLASS.Succeeded())
	{
		MasterSoundClass = BP_MASTER_SOUND_CLASS.Object;
	}
	// 유저가 저장할수있는 최대의 슬롯수를 지정합니다.
	MaxSaveSlotNum =10;

}

void UHInstance::OnStart()
{
	Super::OnStart();

	// 저장된 옵션값을 불러옵니다.
	OptionSaveSystem = OptionLoad();

	// 불러온 옵션값을 지정해주고 옵션을 초기화합니다.
	SetAllMuteStates(OptionSaveSystem->MuteStates);
	SetAllSoundValues(OptionSaveSystem->SoundsValues);
	InitSoundsOption();
	InitGameSettings(OptionSaveSystem->ScreenResolution, EWindowModeFromString(OptionSaveSystem->WindowMode));
}

EWindowMode::Type UHInstance::EWindowModeFromString(FString modeString)
{
	// 인수로 들어온 스트링을 통해 현재의 창모드상태를 확인하고 반환합니다.
	if (modeString == TEXT("전체모드"))
	{
		return EWindowMode::Fullscreen;
	}
	else if (modeString == TEXT("전체창모드"))
	{
		return EWindowMode::WindowedFullscreen;
	}
	else if (modeString == TEXT("창모드"))
	{
		return EWindowMode::Windowed;
	}

	return EWindowMode::Windowed;
}

FString UHInstance::EWindowModeToString(EWindowMode::Type targetMode)
{

	//현재 창모드를 스트링 형식으로 변환합니다.
	switch (targetMode)
	{
	case EWindowMode::Fullscreen:
		return TEXT("전체모드");
		break;
	case EWindowMode::WindowedFullscreen:
		return TEXT("전체창모드");
		break;
	case EWindowMode::Windowed:
		return TEXT("창모드");
		break;
	case EWindowMode::NumWindowModes:
		return TEXT("창모드");
		break;
	default:
		return TEXT("창모드");
		break;
	}
}

void UHInstance::SaveData(int playerIndex)
{
	// 게임세이브 시스템을 생성합니다.
	GameSaveSystem = Cast<UHSaveSystem>(UGameplayStatics::CreateSaveGameObject(UHSaveSystem::StaticClass()));

	// 슬롯이름과 인덱스를 입력된 값으로 지정해줍니다.
	GameSaveSystem->SaveSlotName = FString::FromInt(playerIndex);
	GameSaveSystem->SaveIndex = playerIndex;

	// 플레이어인포를 저장합니다.
	GameSaveSystem->PlayerInfo = GetPlayerCharacter()->GetPlayerInfo();

	// 저장된 세이브시스템을 파일로 저장합니다.
	UGameplayStatics::SaveGameToSlot(GameSaveSystem, GameSaveSystem->SaveSlotName, GameSaveSystem->SaveIndex);

}

void UHInstance::SaveCurrentData()
{
	// 세이브 시스템을 생성합니다.
	GameSaveSystem = Cast<UHSaveSystem>(UGameplayStatics::CreateSaveGameObject(UHSaveSystem::StaticClass()));

	// 현재 인덱스번호로 정보를 저장합니다.
	GameSaveSystem->SaveSlotName = FString::FromInt(CurrentPlayerIndex);
	GameSaveSystem->SaveIndex = CurrentPlayerIndex;

	// 플레이어 정보를 저장합니다.
	GameSaveSystem->PlayerInfo = GetPlayerCharacter()->GetPlayerInfo();

	// 저장된 정보들을 파일로 저장합니다.
	UGameplayStatics::SaveGameToSlot(GameSaveSystem, GameSaveSystem->SaveSlotName, GameSaveSystem->SaveIndex);
}

UHSaveSystem* UHInstance::LoadData(int playerIndex)
{
	// 들어온 값으로 세이브파일의 로드를 시도합니다.
	GameSaveSystem = Cast<UHSaveSystem>(UGameplayStatics::LoadGameFromSlot(FString::FromInt(playerIndex), playerIndex));

	// 만약 실패했다면 초기값으로 초기화시켜서 값을 돌려줍니다.
	if (GameSaveSystem == nullptr)
	{

		GameSaveSystem = Cast<UHSaveSystem>(UGameplayStatics::CreateSaveGameObject(UHSaveSystem::StaticClass()));

		GameSaveSystem->SaveSlotName = FString::FromInt(playerIndex);
		GameSaveSystem->SaveIndex = playerIndex;
		GameSaveSystem->PlayerInfo.ResetInfo();
		GameSaveSystem->PlayerInfo.Level = 0;
				
	}

	return GameSaveSystem;
}

void UHInstance::CreateNewSaveData(int playerIndex, FName characterName)
{
	// 세이브시스템을 생성합니다.
	GameSaveSystem = Cast<UHSaveSystem>(UGameplayStatics::CreateSaveGameObject(UHSaveSystem::StaticClass()));

	// 들어온 인덱스값으로 플레이어 슬롯이름과 인덱스를 지정하고 정보를 초기화한 후 플레이어이름을 넣어줍니다.
	GameSaveSystem->SaveSlotName = FString::FromInt(playerIndex);
	GameSaveSystem->SaveIndex = playerIndex;
	GameSaveSystem->PlayerInfo.ResetInfo();
	GameSaveSystem->PlayerInfo.CharacterName = characterName;

	// 초기화가 완료된 정보를 파일로 저장합니다.
	UGameplayStatics::SaveGameToSlot(GameSaveSystem, GameSaveSystem->SaveSlotName, GameSaveSystem->SaveIndex);
	
}

int UHInstance::GetEmptySaveSlotNum()
{
	// 세이브시스템을 전부 로드해가면서 비어있는 칸을 찾아냅니다.
	for (int i = 0; i < MaxSaveSlotNum; ++i)
	{
		GameSaveSystem = Cast<UHSaveSystem>(UGameplayStatics::LoadGameFromSlot(FString::FromInt(i), i));
		if (GameSaveSystem == nullptr)
		{
			return i;
		}
	}

	//못찾았다면 -1을 반환합니다.
	return -1;
}


UOptionSaveSystem* UHInstance::OptionLoad()
{
	// 옵션 세이브 파일을 불러옵니다.
	OptionSaveSystem = Cast<UOptionSaveSystem>(UGameplayStatics::LoadGameFromSlot(TEXT("Option_0"), 0));

	// 옵션파일이 존재하지 않는다면 생성후 초기화해서 반환합니다.
	if (OptionSaveSystem == nullptr)
	{
		L(TEXT("OptionSaveSystem is Nullptr!"));
		OptionSaveSystem = Cast<UOptionSaveSystem>(UGameplayStatics::CreateSaveGameObject(UOptionSaveSystem::StaticClass()));
		OptionSaveSystem->SaveSlotName = TEXT("Option_0");
		OptionSaveSystem->SaveIndex = 0;

		for (int i = 0; i < 4; ++i)
		{
			OptionSaveSystem->MuteStates.Add(true);
			OptionSaveSystem->SoundsValues.Add(1.0f);
		}
		OptionSaveSystem->ScreenResolution = FIntPoint(1920.0f, 1080.0f);
		OptionSaveSystem->WindowMode = TEXT("창모드");

		UGameplayStatics::SaveGameToSlot(OptionSaveSystem, OptionSaveSystem->SaveSlotName, OptionSaveSystem->SaveIndex);
	}

	return OptionSaveSystem;
}




void UHInstance::SetPlayerController( ACharacterController* playerController)
{
	// 플레이어컨트롤러에서 현재 활동중인 캐릭터와 컨트롤러를 저장하기 위한 구문입니다.
	PlayerController = playerController;
}

void UHInstance::SetPlayerCharacter(APlayerCharacter* playerCharacter)
{
	PlayerCharacter = playerCharacter;
}

template<typename T>
void UHInstance::SaveJson(FString fileName, FString keyName, T& value)
{
	// 저장할 폴더의 위치를 지정합니다.
	FString folderPath = FPaths::ProjectDir() + TEXT("/Content/Resources/Json");

	// 저장할 폴더가 없다면 폴더를 생성해줍니다.
	if(!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*folderPath))
		FPlatformFileManager::Get().GetPlatformFile().CreateDirectory(*folderPath);


	// 저장을 위한 객체를 생성합니다.
	FString jsonStr;
	TSharedRef<TJsonWriter<TCHAR>> jsonObject = TJsonWriterFactory<>::Create(&jsonStr);

	// 저장을 시작합니다.
	jsonObject->WriteObjectStart();

	jsonObject->WriteValue(keyName,value);

	// 저장을 끝내고 오브젝트를 닫습니다.
	jsonObject->WriteObjectEnd();
	jsonObject->Close();

	// 저장할 폴더위치와 파일이름을 지정합니다.
	folderPath += TEXT("/") + fileName + (TEXT(".Json"));

	FFileHelper::SaveStringToFile(*jsonStr,*folderPath);

}

void UHInstance::LoadStringJson(FString fileName, FString keyName, FString& value)
{
	//Json 파일을 저장했던 폴더 경로를 저장합니다.
	FString folderPath = FPaths::ProjectDir() + TEXT("/Content/Resources/Json");

	// 저장을 했던 폴더가 존재하지 않다면 로드를 취소합니다.
	if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*folderPath))
		return;

	// 불러올 파일의 이름을 저장합니다.
	folderPath += TEXT("/") + fileName + (TEXT(".Json"));

	// 파일이름을 토대로 스트링에 Json 파일을 읽어옵니다.
	FString jsonStr;
	if (!FFileHelper::LoadFileToString(jsonStr, *folderPath))
		return;

	// 파일을 읽을 리더를 생성합니다.
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(jsonStr);

	// 공유 가능 포인터를 생성합니다.
	TSharedPtr<FJsonObject> jsonObj = MakeShareable(new FJsonObject());

	// 리더와 공유포인터를 통하여 파일을 역직렬화 합니다.
	if (!FJsonSerializer::Deserialize(reader, jsonObj))
		return;

	// 스트링파일 읽기를 시도합니다.
	jsonObj->TryGetStringField(keyName,value);
	
}

void UHInstance::LoadTArrayStringJson(FString fileName, FString keyName, TArray<FString>& value)
{
	//Json 파일을 저장했던 폴더 경로를 저장합니다.
	FString folderPath = FPaths::ProjectDir() + TEXT("/Content/Resources/Json");

	// 저장을 했던 폴더가 존재하지 않다면 로드를 취소합니다.
	if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*folderPath))
		return;

	// 불러올 파일명을 저장합니다.
	folderPath += TEXT("/") + fileName + (TEXT(".Json"));

	// 파일명을 통해 저장된 파일을 String에 저장합니다.
	FString jsonStr;
	if (!FFileHelper::LoadFileToString(jsonStr, *folderPath))
		return;

	// String에 저장된 파일을 읽기위해 reader를 생성합니다.
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(jsonStr);

	// 오브젝트를 지칭할 공유 포인터를 생성합니다.
	TSharedPtr<FJsonObject> jsonObj = MakeShareable(new FJsonObject());

	// 역직렬화를 합니다.
	if (!FJsonSerializer::Deserialize(reader, jsonObj))
		return;

	jsonObj->TryGetStringArrayField(keyName, value);
}

void UHInstance::DeleteSaveFile(int deleteFileIndex)
{
	//세이브파일위치와 이름을 설정합니다.
	FString fileName = FPaths::ProjectDir() + TEXT("/Saved/SaveGames/") + FString::FromInt(deleteFileIndex) + TEXT(".sav");

	// 설정된 이름으로 된 파일을 삭제합니다.
	FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*fileName);

}

void UHInstance::SetMainWidget(UMainWidget* mainWidget)
{
	MainWidget = mainWidget;
}

void UHInstance::SetHomeWidget(UHomeWidget* homeWidget)
{
	HomeWidget = homeWidget;
}

void UHInstance::SetAllMuteStates(TArray<bool> newMuteStates)
{
	// 음소거 상태를 전체적으로 저장합니다.
	if (MuteStates.Num() < newMuteStates.Num())
	{
		// 현재 저장이 되어야하는 배열이 4개미만이어서 새 데이터와의 배열 수가 맞지 않으면 배열을 추가해줍니다.
		int addNum = newMuteStates.Num() - MuteStates.Num();

		for (int i = 0; i < addNum; ++i)
		{
			MuteStates.Add(false);
		}
	}

	MuteStates = newMuteStates;
}

void UHInstance::InitSoundsOption()
{
	// 게임이 시작 되었을때 음소거 상태에 따라 현재의 사운드 크기를 재 지정해줍니다.
	MasterSoundClass->Properties.Volume = SoundsValues[0] * MuteStates[0];
	MasterSoundClass->ChildClasses[0]->Properties.Volume = SoundsValues[1] * MuteStates[1];
	MasterSoundClass->ChildClasses[1]->Properties.Volume = SoundsValues[2] * MuteStates[2];
	MasterSoundClass->ChildClasses[2]->Properties.Volume = SoundsValues[3] * MuteStates[3];
}

void UHInstance::SetAllSoundValues(TArray<float> newValues)
{
	// 모든 사운드의 크기를 저장을 해줍니다.
	if (SoundsValues.Num() < newValues.Num())
	{
		int addNum = newValues.Num() - SoundsValues.Num();
		for (int i = 0; i < addNum; ++i)
		{
			SoundsValues.Add(1);
		}
	}

	SoundsValues = newValues;
}

void UHInstance::SaveOptions(TArray<float> soundsValue, TArray<bool> muteState)
{
	// 변수로 들어온 값으로 모든 값을 저장합니다.
	SetAllSoundValues(soundsValue);
	SetAllMuteStates(muteState);

	//세이브 시스템을 생성합니다.
	OptionSaveSystem = Cast<UOptionSaveSystem>(UGameplayStatics::CreateSaveGameObject(UOptionSaveSystem::StaticClass()));

	// 세이브 슬롯 이름과 인덱스를 지정합니다.
	OptionSaveSystem->SaveSlotName = TEXT("Option_0");
	OptionSaveSystem->SaveIndex = 0;

	// 저장될 옵션값들을 넣습니다.
	OptionSaveSystem->MuteStates		= GetAllMuteStates();
	OptionSaveSystem->SoundsValues		= GetAllSoundValues();
	OptionSaveSystem->ScreenResolution	= GameSetting->GetScreenResolution();
	OptionSaveSystem->WindowMode		= GameSetting->GetScreenModeToString();

	// 저장이 완료된 시스템을 파일로 저장합니다.
	UGameplayStatics::SaveGameToSlot(OptionSaveSystem, TEXT("Option_0"), 0);

}

void UHInstance::SetCurrentPlayerIndex(int playerIndex)
{
	CurrentPlayerIndex = playerIndex;
}

int UHInstance::GetCurrentPlayerIndex()
{
	return CurrentPlayerIndex;
}

void UHInstance::InitGameSettings(FIntPoint screenResolution, EWindowMode::Type windowMode)
{
	// 게임세팅을 초기화합니다.
	GameSetting->InitGameSetting(GetEngine()->GetGameUserSettings());
	GameSetting->SetScreenResolution(screenResolution);
	GameSetting->SetScreenMode(windowMode);
}

void UHInstance::OpenNewMap(FName mapName)
{
	// 새로 열어야 할 맵이름을 변경해줍니다.
	NewMapName = mapName;
	// 새 맵을 열기전 로딩맵을 열어줍니다.
	FName newLoading = FName(TEXT("LoadingLevel"));
	UGameplayStatics::OpenLevel(this, newLoading);
}

void UHInstance::OpenNextMap()
{
	// 로딩맵에서 실제로 열려고 했던 맵으로 변경합니다.
	UGameplayStatics::OpenLevel(this, NewMapName);
}

