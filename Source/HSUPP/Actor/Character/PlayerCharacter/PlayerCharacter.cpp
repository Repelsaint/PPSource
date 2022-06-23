// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Actor/Controller/CharacterController.h"
#include "Actor/Character/BaseNpc/BaseNpc.h"

#include "Anim/PlayerAnimInst/PlayerAnimInst.h"

#include "Camera/CameraComponent.h"

#include "Kismet/GameplayStatics.h"

#include "GameMode/HGameMode.h"

#include "Single/HSaveSystem/HSaveSystem.h"

#include "GameFramework/PlayerStart.h"

#include "Widget/MainWidget.h"

#include "Component/InteractableAreaComponent/InteractableAreaComponent.h"


#ifndef GET_PLAYER_INFO
#define GET_PLAYER_INFO

#define LoadData(indexNum)  GetHInstance()->LoadData(indexNum)
#define SaveData(indexNum)  GetHInstance()->SaveData(indexNum)

#endif


APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	
	//팀을 플레이어로 지정합니다.
	SetGenericTeamId(ETeam::Player);

	// 캐릭터의 메쉬를 불러와서 적용시킵니다.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CHARACTERMESH(TEXT("SkeletalMesh'/Game/Resources/Character/Berserker_S1/Meshes/Character/Combines/SK_BersekerS1_Weapon.SK_BersekerS1_Weapon'"));
	if (CHARACTERMESH.Succeeded())
		CharacterMesh = CHARACTERMESH.Object;
	else
		L(TEXT("Mesh Load Failed PlayerCharacter.cpp %d Line"), __LINE__);
	
	// 플레이어의 애니메이션인스턴스를 불러와서 적용시킵니다.
	static ConstructorHelpers::FClassFinder<UPlayerAnimInst> BP_PLAYER_ANIM_INST(TEXT("AnimBlueprint'/Game/Anim/PlayerCharacterAnim/AB_PlayerCharacter.AB_PlayerCharacter_C'"));
	if(BP_PLAYER_ANIM_INST.Succeeded())
		PlayerAnimClass = BP_PLAYER_ANIM_INST.Class;

	// 플레이어의 기본 컴포넌트들을 생성하고 지정해줍니다.
	AttackComponent = CreateDefaultSubobject<UPlayerAttack>(TEXT("PLAYER_ATTACK"));
	SpringArm = CreateDefaultSubobject<UZoomableSpringArm>(TEXT("SPRING_ARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	PlayerMoveComponent = Cast<UPlayerMovementComponent>(GetMovementComponent());
	SpringArm->SetupAttachment(GetRootComponent());
	Camera->SetupAttachment(SpringArm);

	// 회전시 컨트롤러의 Yaw를 받지 않도록 지정합니다.
	bUseControllerRotationYaw = false;

	// 애니메이션 클래스를 아까 불러왔던 애니메이션으로 변경해줍니다.
	GetMesh()->SetAnimClass(PlayerAnimClass);

	// 캐릭터가 이동하는 방향으로 머리를 돌릴수 있도록 합니다.
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// 회전속도를 지정합니다.
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 800.0f, 0.0f);

	// 공중에서 이동방향을 거의 바꿀수 없도록 지정합니다.
	GetCharacterMovement()->AirControl = 0.1f;

	// 메쉬는 노콜리젼으로하고 캡슐컴포넌트를 플레이어콜리젼으로 지정하여 데미지 처리가 쉽도록 하였습니다.
	GetMesh()->SetCollisionProfileName(FName("NoCollision"));
	GetCapsuleComponent()->SetCollisionProfileName(FName("PlayerCharacter"));

	// 스켈레탈 메쉬를 불러들였던 값으로 지정합니다.
	GetMesh()->SetSkeletalMesh(CharacterMesh);

	// 메쉬의 위치를 올바르게 지정합니다.
	GetMesh()->SetRelativeLocationAndRotation(
		FVector::DownVector * 88.0f,
		FRotator(0.0f, -90.0f, 0.0f));
	
	// 기본값이 트루여야하는 bool값들을 초기화해줍니다.
	bCanMove = true;
	bAlive = true;
}

void APlayerCharacter::TryAttack()
{
	// 호출시 어택컴포넌트의 공격시도메서드를 호출합니다.
	AttackComponent->TryNormalAttack();
}



void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	// 게임 시작시 플레이어의 정보를 읽어옵니다.
	LoadPlayerInfo();
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Forward"), GetPlayerMovementComponent(), &UPlayerMovementComponent::PlayerMoveForward);
	PlayerInputComponent->BindAxis(TEXT("Right"), GetPlayerMovementComponent(), &UPlayerMovementComponent::PlayerMoveRight);
	PlayerInputComponent->BindAxis(TEXT("Zoom"), SpringArm, &UZoomableSpringArm::AddArmLength);

	PlayerInputComponent->BindAction(TEXT("Jump"),EInputEvent::IE_Pressed, GetPlayerMovementComponent(), &UPlayerMovementComponent::PlayerJump);
	PlayerInputComponent->BindAction(TEXT("Run"),EInputEvent::IE_Pressed, GetPlayerMovementComponent(), &UPlayerMovementComponent::PlayerRunStart);
	PlayerInputComponent->BindAction(TEXT("Run"),EInputEvent::IE_Released, GetPlayerMovementComponent(), &UPlayerMovementComponent::PlayerRunEnd);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &APlayerCharacter::TryAttack);
}

void APlayerCharacter::GetDamaged(int damaged)
{
	Super::GetDamaged(damaged);

	//BaseCharacter에 있는 HP 가 Info에 저장된 HP값과 다르므로 Info의 HP가 실제와 같도록 깎아줍니다.
	PlayerInfo.CurrentHP -= damaged;

	// 체력이 0 이하로 떨어질경우
	if (PlayerInfo.CurrentHP < 0)
	{
	// 체력을 0으로 바꿔주고 사망한것으로 값을 변경합니다.
		PlayerInfo.CurrentHP = 0;
		bAlive = false;
	}

	// 변경된 HP값을 토대로 UI를 조정해줍니다.
	GetHInstance()->GetMainWidget()->SetHPBar(PlayerInfo.CurrentHP, PlayerInfo.MaxHP);
}

void APlayerCharacter::CharacterDie()
{
	Super::CharacterDie();
	
	// 죽었을때 내는 소리를 읽어와서 실행시킵니다.
	USoundWave* deathSound = Cast<USoundWave>(GetStreamManager()->LoadSynchronous(FSoftClassPath(TEXT("/Game/Resources/UsingSound/UserSound/S_PlayerDeath.S_PlayerDeath"))));
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), deathSound,GetActorLocation());
	
	//사망 몽타쥬를 실행시킵니다.
	PlayAnimMontage(Cast<UAnimMontage>(GetStreamManager()->LoadSynchronous(FSoftClassPath(TEXT("/Game/Anim/PlayerCharacterAnim/AM_PCDie.AM_PCDie")))));
}

void APlayerCharacter::LoadPlayerInfo()
{
	// 현재 선택된 플레이어의 정보를 읽어와서 저장합니다.
	PlayerInfo = LoadData(GetHInstance()->GetCurrentPlayerIndex())->PlayerInfo;

	// BaseCharacter에서 실행되는 GetDamaged의 원활한 처리를 위하여 CurrentHP를 변경해줍니다.
	CurrentHP = PlayerInfo.CurrentHP;
}

void APlayerCharacter::PlayerRespawn()
{
	// 사망하였으므로 캐릭터를 마을 맵으로 옮겨줍니다.
	GetHInstance()->OpenNewMap(FName(TEXT("VilageLevel")));

	// 마을 레벨상의 플레이어 위치 시작점을 읽어오고 액터의 위치를 변경해줍니다.
	TArray<AActor*> playerStart;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("StartActor")), playerStart);
	FVector newVector = Cast<APlayerStart>(playerStart[0])->GetActorLocation();
	SetActorLocation(newVector);

	// 사망처리를 위해 꺼두었던 기능들을 다시 켜줍니다.
	GetMesh()->SetVisibility(true);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerCharacter"));
	SetMovable(true);
	bAlive = true;

	// 현재 체력을 최대체력으로 초기화해줍니다.
	CurrentHP = PlayerInfo.CurrentHP = PlayerInfo.MaxHP;
	
	// 공격중 사망했을때를 대비해서 초기화해줍니다.
	AttackComponent->ResetAllSkill();
	
	// 정보의 초기화가 끝난후 체력바를 재 지정해줍니다.
	GetHInstance()->GetMainWidget()->SetHPBar(PlayerInfo.CurrentHP, PlayerInfo.MaxHP);

}

void APlayerCharacter::AddInteractableActor(UInteractableAreaComponent* newActor)
{
	// 상호작용가능한 액터 배열이 비어있다면
	if (InteractableActors.Num() < 1)
	{
		// 메인위젯에 상호작용가능 메시지를 띄우도록합니다.
		GetHInstance()->GetMainWidget()->CreateInteractMessage(Cast<ABaseNpc>(newActor->GetOwner()));
	}

	//상호작용가능액터로 추가해줍니다.
	InteractableActors.Add(newActor);
}

void APlayerCharacter::RemoveInteractableActor(UInteractableAreaComponent* deleteActor)
{
	//배열에서 액터를 제거합니다.
	InteractableActors.Remove(deleteActor);

	// 액터를 제거함으로서 상호작용 가능한 액터가 존재하지 않는다면
	if (InteractableActors.Num() == 0)
	{
		// 켜져있는 상호작용 가능 메세지를 삭제합니다.
		GetHInstance()->GetMainWidget()->DeleteInteractMessage(Cast<ABaseNpc>(deleteActor->GetOwner()));
	}
}