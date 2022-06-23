

#include "NormalLevelActor.h"
#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"
#include "Actor/EnemySpawnArea/EnemySpawnArea.h"
#include "Actor/PoolableParticleSystem//PoolableParticleSystem.h"

ANormalLevelActor::ANormalLevelActor()
{
	// 레벨 타입을 일반으로 지정해줍니다.
	LevelType = ELevelActorType::Normal;
	
	// 노말 레벨에는 마을과 던전이 존재하기때문에 각각 실행될 사운드를 읽어와서 저장합니다.
	static ConstructorHelpers::FObjectFinder<USoundWave> SW_DL(TEXT("SoundWave'/Game/Resources/UsingSound/BackGround/Elven_Harmonies.Elven_Harmonies'"));
	if(SW_DL.Succeeded())
		DungeonSound = SW_DL.Object;

	static ConstructorHelpers::FObjectFinder<USoundWave> SW_VL(TEXT("SoundWave'/Game/Resources/UsingSound/BackGround/Magical_Fantasy.Magical_Fantasy'"));
	if (SW_VL.Succeeded())
		VillageSound = SW_VL.Object;
		
}

void ANormalLevelActor::BeginPlay()
{
	Super::BeginPlay();

	// 오브젝트 풀링 컴포넌트를 생성하여줍니다.
	ObjectPoolComponent = NewObject<UObjectPool>();

	// 맵 이름을 읽어와서 맵에 알맞은 음악을 틀어줍니다.
	if (GetWorld()->GetName() == TEXT("DungeonLevel"))
	{
		LevelBackgroundMusic = UGameplayStatics::CreateSound2D(GetWorld(), DungeonSound);
		LevelBackgroundMusic->Play();

	}
	else if (GetWorld()->GetName() == TEXT("VilageLevel"))
	{
		LevelBackgroundMusic = UGameplayStatics::CreateSound2D(GetWorld(), VillageSound);
		LevelBackgroundMusic->Play();

	}

}

void ANormalLevelActor::AddSpawnArea(AEnemySpawnArea* spawnArea)
{
	//인자로 들어온 액터를 배열에 추가합니다.
	SpawnAreas.Add(spawnArea);
}

AEnemySpawnArea* ANormalLevelActor::GetSpawnAreaArray(int enemyCode)
{
	//배열속에서 적 코드와 같은 스폰박스를 찾아내서 반환합니다.
	for (int i = 0; i < SpawnAreas.Num(); ++i)
	{
		if (SpawnAreas[i]->GetAreaEnemyCode() == enemyCode)
		{
			return SpawnAreas[i];
		}
	}

	return nullptr;
}

APoolableParticleSystem* ANormalLevelActor::SpawnParticle(UParticleSystem* particleSystem, FVector spawnLocation, FRotator spawnRotation)
{
	// 레벨 액터에 등록된 재사용가능 객체 배열에 저장된 사용가능한 파티클객체를 얻어옵니다.
	APoolableParticleSystem* newParticle = ObjectPoolComponent->GetRecyclableObject<APoolableParticleSystem>();

	// 만약 존재하지 않는다면 새로 액터를 생성하여 할당해줍니다.
	if (newParticle == nullptr)
	{
		newParticle = GetWorld()->SpawnActor<APoolableParticleSystem>(APoolableParticleSystem::StaticClass());
	}


	// 생성 혹은 재사용이 된 액터의 위치와 방향을 지정해줍니다.
	newParticle->SetActorLocation(spawnLocation);
	newParticle->SetActorRotation(spawnRotation);


	// 파티클액터를 배열에 저장합니다.
	ObjectPoolComponent->RegisterPoolableObject(newParticle);

	//파티클을 지정하고 실행시켜줍니다.
	newParticle->GetParticleSystem()->SetTemplate(particleSystem);
	newParticle->GetParticleSystem()->Activate(true);

	// 만들어진 파티클을 반환합니다.
	return newParticle;
}

void ANormalLevelActor::SetBGMMusicSound(float soundSize)
{
	//볼륨 크기를 변경합니다.
	LevelBackgroundMusic->SetVolumeMultiplier(soundSize);
}
