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
	
	//���� �÷��̾�� �����մϴ�.
	SetGenericTeamId(ETeam::Player);

	// ĳ������ �޽��� �ҷ��ͼ� �����ŵ�ϴ�.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CHARACTERMESH(TEXT("SkeletalMesh'/Game/Resources/Character/Berserker_S1/Meshes/Character/Combines/SK_BersekerS1_Weapon.SK_BersekerS1_Weapon'"));
	if (CHARACTERMESH.Succeeded())
		CharacterMesh = CHARACTERMESH.Object;
	else
		L(TEXT("Mesh Load Failed PlayerCharacter.cpp %d Line"), __LINE__);
	
	// �÷��̾��� �ִϸ��̼��ν��Ͻ��� �ҷ��ͼ� �����ŵ�ϴ�.
	static ConstructorHelpers::FClassFinder<UPlayerAnimInst> BP_PLAYER_ANIM_INST(TEXT("AnimBlueprint'/Game/Anim/PlayerCharacterAnim/AB_PlayerCharacter.AB_PlayerCharacter_C'"));
	if(BP_PLAYER_ANIM_INST.Succeeded())
		PlayerAnimClass = BP_PLAYER_ANIM_INST.Class;

	// �÷��̾��� �⺻ ������Ʈ���� �����ϰ� �������ݴϴ�.
	AttackComponent = CreateDefaultSubobject<UPlayerAttack>(TEXT("PLAYER_ATTACK"));
	SpringArm = CreateDefaultSubobject<UZoomableSpringArm>(TEXT("SPRING_ARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	PlayerMoveComponent = Cast<UPlayerMovementComponent>(GetMovementComponent());
	SpringArm->SetupAttachment(GetRootComponent());
	Camera->SetupAttachment(SpringArm);

	// ȸ���� ��Ʈ�ѷ��� Yaw�� ���� �ʵ��� �����մϴ�.
	bUseControllerRotationYaw = false;

	// �ִϸ��̼� Ŭ������ �Ʊ� �ҷ��Դ� �ִϸ��̼����� �������ݴϴ�.
	GetMesh()->SetAnimClass(PlayerAnimClass);

	// ĳ���Ͱ� �̵��ϴ� �������� �Ӹ��� ������ �ֵ��� �մϴ�.
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// ȸ���ӵ��� �����մϴ�.
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 800.0f, 0.0f);

	// ���߿��� �̵������� ���� �ٲܼ� ������ �����մϴ�.
	GetCharacterMovement()->AirControl = 0.1f;

	// �޽��� ���ݸ��������ϰ� ĸ��������Ʈ�� �÷��̾��ݸ������� �����Ͽ� ������ ó���� ������ �Ͽ����ϴ�.
	GetMesh()->SetCollisionProfileName(FName("NoCollision"));
	GetCapsuleComponent()->SetCollisionProfileName(FName("PlayerCharacter"));

	// ���̷�Ż �޽��� �ҷ��鿴�� ������ �����մϴ�.
	GetMesh()->SetSkeletalMesh(CharacterMesh);

	// �޽��� ��ġ�� �ùٸ��� �����մϴ�.
	GetMesh()->SetRelativeLocationAndRotation(
		FVector::DownVector * 88.0f,
		FRotator(0.0f, -90.0f, 0.0f));
	
	// �⺻���� Ʈ�翩���ϴ� bool������ �ʱ�ȭ���ݴϴ�.
	bCanMove = true;
	bAlive = true;
}

void APlayerCharacter::TryAttack()
{
	// ȣ��� ����������Ʈ�� ���ݽõ��޼��带 ȣ���մϴ�.
	AttackComponent->TryNormalAttack();
}



void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	// ���� ���۽� �÷��̾��� ������ �о�ɴϴ�.
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

	//BaseCharacter�� �ִ� HP �� Info�� ����� HP���� �ٸ��Ƿ� Info�� HP�� ������ ������ ����ݴϴ�.
	PlayerInfo.CurrentHP -= damaged;

	// ü���� 0 ���Ϸ� ���������
	if (PlayerInfo.CurrentHP < 0)
	{
	// ü���� 0���� �ٲ��ְ� ����Ѱ����� ���� �����մϴ�.
		PlayerInfo.CurrentHP = 0;
		bAlive = false;
	}

	// ����� HP���� ���� UI�� �������ݴϴ�.
	GetHInstance()->GetMainWidget()->SetHPBar(PlayerInfo.CurrentHP, PlayerInfo.MaxHP);
}

void APlayerCharacter::CharacterDie()
{
	Super::CharacterDie();
	
	// �׾����� ���� �Ҹ��� �о�ͼ� �����ŵ�ϴ�.
	USoundWave* deathSound = Cast<USoundWave>(GetStreamManager()->LoadSynchronous(FSoftClassPath(TEXT("/Game/Resources/UsingSound/UserSound/S_PlayerDeath.S_PlayerDeath"))));
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), deathSound,GetActorLocation());
	
	//��� ��Ÿ�긦 �����ŵ�ϴ�.
	PlayAnimMontage(Cast<UAnimMontage>(GetStreamManager()->LoadSynchronous(FSoftClassPath(TEXT("/Game/Anim/PlayerCharacterAnim/AM_PCDie.AM_PCDie")))));
}

void APlayerCharacter::LoadPlayerInfo()
{
	// ���� ���õ� �÷��̾��� ������ �о�ͼ� �����մϴ�.
	PlayerInfo = LoadData(GetHInstance()->GetCurrentPlayerIndex())->PlayerInfo;

	// BaseCharacter���� ����Ǵ� GetDamaged�� ��Ȱ�� ó���� ���Ͽ� CurrentHP�� �������ݴϴ�.
	CurrentHP = PlayerInfo.CurrentHP;
}

void APlayerCharacter::PlayerRespawn()
{
	// ����Ͽ����Ƿ� ĳ���͸� ���� ������ �Ű��ݴϴ�.
	GetHInstance()->OpenNewMap(FName(TEXT("VilageLevel")));

	// ���� �������� �÷��̾� ��ġ �������� �о���� ������ ��ġ�� �������ݴϴ�.
	TArray<AActor*> playerStart;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("StartActor")), playerStart);
	FVector newVector = Cast<APlayerStart>(playerStart[0])->GetActorLocation();
	SetActorLocation(newVector);

	// ���ó���� ���� ���ξ��� ��ɵ��� �ٽ� ���ݴϴ�.
	GetMesh()->SetVisibility(true);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerCharacter"));
	SetMovable(true);
	bAlive = true;

	// ���� ü���� �ִ�ü������ �ʱ�ȭ���ݴϴ�.
	CurrentHP = PlayerInfo.CurrentHP = PlayerInfo.MaxHP;
	
	// ������ ����������� ����ؼ� �ʱ�ȭ���ݴϴ�.
	AttackComponent->ResetAllSkill();
	
	// ������ �ʱ�ȭ�� ������ ü�¹ٸ� �� �������ݴϴ�.
	GetHInstance()->GetMainWidget()->SetHPBar(PlayerInfo.CurrentHP, PlayerInfo.MaxHP);

}

void APlayerCharacter::AddInteractableActor(UInteractableAreaComponent* newActor)
{
	// ��ȣ�ۿ밡���� ���� �迭�� ����ִٸ�
	if (InteractableActors.Num() < 1)
	{
		// ���������� ��ȣ�ۿ밡�� �޽����� ��쵵���մϴ�.
		GetHInstance()->GetMainWidget()->CreateInteractMessage(Cast<ABaseNpc>(newActor->GetOwner()));
	}

	//��ȣ�ۿ밡�ɾ��ͷ� �߰����ݴϴ�.
	InteractableActors.Add(newActor);
}

void APlayerCharacter::RemoveInteractableActor(UInteractableAreaComponent* deleteActor)
{
	//�迭���� ���͸� �����մϴ�.
	InteractableActors.Remove(deleteActor);

	// ���͸� ���������μ� ��ȣ�ۿ� ������ ���Ͱ� �������� �ʴ´ٸ�
	if (InteractableActors.Num() == 0)
	{
		// �����ִ� ��ȣ�ۿ� ���� �޼����� �����մϴ�.
		GetHInstance()->GetMainWidget()->DeleteInteractMessage(Cast<ABaseNpc>(deleteActor->GetOwner()));
	}
}