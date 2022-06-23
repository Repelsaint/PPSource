

#include "NormalLevelActor.h"
#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"
#include "Actor/EnemySpawnArea/EnemySpawnArea.h"
#include "Actor/PoolableParticleSystem//PoolableParticleSystem.h"

ANormalLevelActor::ANormalLevelActor()
{
	// ���� Ÿ���� �Ϲ����� �������ݴϴ�.
	LevelType = ELevelActorType::Normal;
	
	// �븻 �������� ������ ������ �����ϱ⶧���� ���� ����� ���带 �о�ͼ� �����մϴ�.
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

	// ������Ʈ Ǯ�� ������Ʈ�� �����Ͽ��ݴϴ�.
	ObjectPoolComponent = NewObject<UObjectPool>();

	// �� �̸��� �о�ͼ� �ʿ� �˸��� ������ Ʋ���ݴϴ�.
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
	//���ڷ� ���� ���͸� �迭�� �߰��մϴ�.
	SpawnAreas.Add(spawnArea);
}

AEnemySpawnArea* ANormalLevelActor::GetSpawnAreaArray(int enemyCode)
{
	//�迭�ӿ��� �� �ڵ�� ���� �����ڽ��� ã�Ƴ��� ��ȯ�մϴ�.
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
	// ���� ���Ϳ� ��ϵ� ���밡�� ��ü �迭�� ����� ��밡���� ��ƼŬ��ü�� ���ɴϴ�.
	APoolableParticleSystem* newParticle = ObjectPoolComponent->GetRecyclableObject<APoolableParticleSystem>();

	// ���� �������� �ʴ´ٸ� ���� ���͸� �����Ͽ� �Ҵ����ݴϴ�.
	if (newParticle == nullptr)
	{
		newParticle = GetWorld()->SpawnActor<APoolableParticleSystem>(APoolableParticleSystem::StaticClass());
	}


	// ���� Ȥ�� ������ �� ������ ��ġ�� ������ �������ݴϴ�.
	newParticle->SetActorLocation(spawnLocation);
	newParticle->SetActorRotation(spawnRotation);


	// ��ƼŬ���͸� �迭�� �����մϴ�.
	ObjectPoolComponent->RegisterPoolableObject(newParticle);

	//��ƼŬ�� �����ϰ� ��������ݴϴ�.
	newParticle->GetParticleSystem()->SetTemplate(particleSystem);
	newParticle->GetParticleSystem()->Activate(true);

	// ������� ��ƼŬ�� ��ȯ�մϴ�.
	return newParticle;
}

void ANormalLevelActor::SetBGMMusicSound(float soundSize)
{
	//���� ũ�⸦ �����մϴ�.
	LevelBackgroundMusic->SetVolumeMultiplier(soundSize);
}
