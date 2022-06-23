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
	//�ν��Ͻ��� ���������� �������Դϴ�.
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

	// ���Ұ� On/Off �� �ϴ��� ������ �Ҹ����� ���� �ʵ��� �����ϴ� �迭
	TArray<float> SoundsValues;

protected:

	virtual void OnStart() override;

public:
	UHInstance();

	// FString �������� ���� ���� ���Ͽ� ������ ��带 ���������ݴϴ�.
	EWindowMode::Type EWindowModeFromString(FString modeString);
	// �������� �������� ���� ���� FString �������� ��ȯ��ȯ�մϴ�.
	FString EWindowModeToString(EWindowMode::Type targetMode);

	//�ε�����ȣ�� �����͸� �����մϴ�.
	void SaveData(int playerIndex);

	// ���� �����ִ� ĳ������ �ε����� ���Ͽ� ������ �մϴ�.
	void SaveCurrentData();
	// �ε����� �ش��ϴ� ������ �ҷ��ɴϴ�.
	UHSaveSystem* LoadData(int playerIndex);

	// ���ο� �����͸� �����ϰ� �ʱ�ȭ�մϴ�.
	void CreateNewSaveData(int playerIndex, FName characterName);

	// ����ִ� ĭ�� ã���� ���˴ϴ�.
	int GetEmptySaveSlotNum();
	
	// �ɼ��� �ҷ��ͼ� �ʱ�ȭ�Ҷ� ���˴ϴ�.
	UOptionSaveSystem* OptionLoad();

	// �ɼ��� ������ �Ҷ� ����� �˴ϴ�.
	void SaveOptions(TArray<float> soundsValue, TArray<bool> muteState);

	// ���� �����ִ� �÷��̾��� �ε����� �����մϴ�.
	void SetCurrentPlayerIndex(int playerIndex);
	
	// ���� �����ִ� �÷��̾��� �ε����� ���ɴϴ�.
	int GetCurrentPlayerIndex();

	// ���� ������ ĳ���Ϳ� ��Ʈ�ѷ��� �����մϴ�.
	void SetPlayerController(ACharacterController* playerController);
	void SetPlayerCharacter(APlayerCharacter* playerCharacter);

	// T �������� ���� ������ Json���� �����մϴ�.
	template<typename T>
	void SaveJson(FString fileName, FString keyName ,T& value);

	// String / TArray<FString> �������� �� Json ������ �о�ɴϴ�.
	void LoadStringJson(FString fileName, FString keyName, FString& value);
	void LoadTArrayStringJson(FString fileName, FString keyName, TArray<FString>& value);

	// ����� ������ �����Ҷ� ����դ���.
	void DeleteSaveFile(int deleteFileIndex);

	// �����ִ� ������ �����Ҷ� ���˴ϴ�.
	void SetMainWidget(class UMainWidget* mainWidget);
	void SetHomeWidget(class UHomeWidget* homeWidget);

	// ����ɼ��� �ʱ�ȭ�մϴ�.
	void InitSoundsOption();

	// ��� ���尪�� �����մϴ�.
	void SetAllSoundValues(TArray<float> newValues);

	// ���Ӽ����� ���� �ʱ�ȭ���ݴϴ�.
	void InitGameSettings(FIntPoint screenResolution, EWindowMode::Type windowMode);

	// ���� ���� ���� ����մϴ�.
	void OpenNewMap(FName mapName);

	// �ε��ʿ��� ���� ���� �ε��մϴ�.
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
