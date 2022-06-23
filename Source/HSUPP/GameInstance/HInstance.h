// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSUPP.h"
#include "Engine/GameInstance.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Sound/SoundClass.h"


#include "HInstance.generated.h"


/**
 * 
 */
UCLASS()
class HSUPP_API UHInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	//인스턴스가 가지고있을 변수들입니다.
	class UGameSettings* GameSetting;
	class APlayerCharacter* PlayerCharacter;
	class ACharacterController* PlayerController;
	class UMainWidget* MainWidget;
	class UHomeWidget* HomeWidget;
	class AHGameMode* HSGameMode;

	class UHSaveSystem* GameSaveSystem;
	class UOptionSaveSystem* OptionSaveSystem;

	USoundClass* MasterSoundClass;

	TArray<bool> MuteStates;

	FName NewMapName;

	int MaxSaveSlotNum;

	int CurrentPlayerIndex;

	// 음소거 On/Off 를 하더라도 기존의 소리값을 잃지 않도록 저장하는 배열
	TArray<float> SoundsValues;

protected:

	virtual void OnStart() override;

public:
	UHInstance();

	// FString 형식으로 들어온 값을 통하여 윈도우 모드를 재지정해줍니다.
	EWindowMode::Type EWindowModeFromString(FString modeString);
	// 윈도우모드 형식으로 들어온 값을 FString 형식으로 변환반환합니다.
	FString EWindowModeToString(EWindowMode::Type targetMode);

	//인덱스번호로 데이터를 저장합니다.
	void SaveData(int playerIndex);

	// 현재 열려있는 캐릭터의 인덱스를 통하여 저장을 합니다.
	void SaveCurrentData();
	// 인덱스에 해당하는 정보를 불러옵니다.
	UHSaveSystem* LoadData(int playerIndex);

	// 새로운 데이터를 저장하고 초기화합니다.
	void CreateNewSaveData(int playerIndex, FName characterName);

	// 비어있는 칸을 찾을때 사용됩니다.
	int GetEmptySaveSlotNum();
	
	// 옵션을 불러와서 초기화할때 사용됩니다.
	UOptionSaveSystem* OptionLoad();

	// 옵션을 저장을 할때 사용이 됩니다.
	void SaveOptions(TArray<float> soundsValue, TArray<bool> muteState);

	// 현재 열려있는 플레이어의 인덱스를 지정합니다.
	void SetCurrentPlayerIndex(int playerIndex);
	
	// 현재 열려있는 플레이어의 인덱스를 얻어옵니다.
	int GetCurrentPlayerIndex();

	// 현재 생성된 캐릭터와 컨트롤러를 지정합니다.
	void SetPlayerController(ACharacterController* playerController);
	void SetPlayerCharacter(APlayerCharacter* playerCharacter);

	// T 형식으로 들어온 정보를 Json으로 저장합니다.
	template<typename T>
	void SaveJson(FString fileName, FString keyName ,T& value);

	// String / TArray<FString> 형식으로 된 Json 파일을 읽어옵니다.
	void LoadStringJson(FString fileName, FString keyName, FString& value);
	void LoadTArrayStringJson(FString fileName, FString keyName, TArray<FString>& value);

	// 저장된 파일을 삭제할때 사용합ㄴ디ㅏ.
	void DeleteSaveFile(int deleteFileIndex);

	// 열려있는 위젯을 저장할때 사용됩니다.
	void SetMainWidget(class UMainWidget* mainWidget);
	void SetHomeWidget(class UHomeWidget* homeWidget);

	// 사운드옵션을 초기화합니다.
	void InitSoundsOption();

	// 모든 사운드값을 저장합니다.
	void SetAllSoundValues(TArray<float> newValues);

	// 게임세팅을 전부 초기화해줍니다.
	void InitGameSettings(FIntPoint screenResolution, EWindowMode::Type windowMode);

	// 다음 맵을 열때 사용합니다.
	void OpenNewMap(FName mapName);

	// 로딩맵에서 다음 맵을 로드합니다.
	void OpenNextMap();

	FORCEINLINE AHGameMode* GetAHGameMode() const
	{ return HSGameMode;}

	FORCEINLINE void SetAHGameMode(AHGameMode* newGameMode)
	{ HSGameMode = newGameMode;}

	FORCEINLINE int GetMaxSlotNum() const
	{ return MaxSaveSlotNum; }

	FORCEINLINE FStreamableManager* GetStreamableManager() const
	{ return &UAssetManager::GetStreamableManager(); }

	FORCEINLINE UMainWidget* GetMainWidget() const
	{ return MainWidget; }

	FORCEINLINE UHomeWidget* GetHomeWidget() const
	{ return HomeWidget; }

	FORCEINLINE APlayerCharacter* GetPlayerCharacter() const
	{ return PlayerCharacter; }

	FORCEINLINE ACharacterController* GetPlayerController() const
	{ return PlayerController; }

	FORCEINLINE UGameSettings* GetGameSetting() const
	{ return GameSetting; }

	FORCEINLINE USoundClass* GetMasterSoundClass() const
	{ return MasterSoundClass; }

	FORCEINLINE bool GetMuteState(int indexNum)
	{ return MuteStates[indexNum]; }

	FORCEINLINE TArray<bool> GetAllMuteStates()
	{ return MuteStates; }

	FORCEINLINE void SetMuteState(int indexNum,bool newMuteState)
	{ MuteStates[indexNum] = newMuteState;}

	void SetAllMuteStates(TArray<bool> newMuteStates);


	FORCEINLINE float& GetSoundValue(int indexNum)
	{ return SoundsValues[indexNum]; }

	FORCEINLINE TArray<float> GetAllSoundValues()
	{ return SoundsValues; }

	FORCEINLINE void SetSoundValue(int indexNum, float newValue)
	{ SoundsValues[indexNum] = newValue; }





};
