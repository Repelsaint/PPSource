// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInst.h"
#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"

UPlayerAnimInst::UPlayerAnimInst()
{	
	// ĳ���Ͱ� �̵��� �� �� ���带 ȣ���մϴ�. ���ʹ߰� �����ʹ��� �߼Ҹ��� ���� �ٸ��� �ϱ� ���ؼ� ���� ���� �ҷ����Դϴ�.
	static ConstructorHelpers::FObjectFinder<USoundWave> FIRST_WALK(TEXT("SoundWave'/Game/Resources/UsingSound/UserSound/running_first.running_first'"));

	if(FIRST_WALK.Succeeded())
		WalkFirstSound = FIRST_WALK.Object;

	static ConstructorHelpers::FObjectFinder<USoundWave> SECOND_WALK(TEXT("SoundWave'/Game/Resources/UsingSound/UserSound/running_Second.running_Second'"));
	if(SECOND_WALK.Succeeded())
	 WalkSecondSound = SECOND_WALK.Object;
}

void UPlayerAnimInst::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// ��������ĳ���͸� �����մϴ�.
	OwnerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
}

void UPlayerAnimInst::NativeUpdateAnimation(float dt)
{
	Super::NativeUpdateAnimation(dt);
	
	// ���� �������� �÷��̾ ���ٸ� �������� �ʽ��ϴ�.
	if (!IsValid(OwnerCharacter)) return;
	
	//������ �÷��̾��� �ӵ��� ���缭 �ִϸ��̼��� ����ǰ� �Ͽ�����
	// wasd�� ���� �Է����� ĳ���Ͱ� ������ ������ �ٲٰ� �Ǹ�
	// �÷��̾ �����̴� �ִϸ��̼� �ӵ��� �������¿� �̸��⶧���� ������ �̵��ӵ��� �����մϴ�.
	//MoveSpeed = OwnerCharacter->GetVelocity().Size();
	if (OwnerCharacter->GetVelocity().Size() > 0.0f && OwnerCharacter->GetVelocity().Size() <= 501.0f)
	{
		MoveSpeed = 500.0f;
	}
	else if (OwnerCharacter->GetVelocity().Size() <= 1000.0f && OwnerCharacter->GetVelocity().Size() > 501.0f)
		MoveSpeed = 1000.0f;
	else if (OwnerCharacter->GetVelocity().Size() == 0.0f)
		MoveSpeed = 0.0f;
	
	
	
	// �÷��̾��� ����������Ʈ�� ���� �÷��̾ �������� �ƴ����� üũ�մϴ�.
	bIsInAir = OwnerCharacter->GetMovementComponent()->IsFalling();
	
	// ���� �����̶�� �����ϼ� ������ �ϰ� ������ ������ �ʾҴٴ°��� üũ�� ���ݴϴ�.	
	if (bIsInAir)
	{
		OwnerCharacter->SetMovable(false);
		bJumpEnd = false;
	}
}

void UPlayerAnimInst::AnimNotify_JumpEnd()
{
	// ���� ������ �÷��̾ �����ϼ� �ֵ��� ���ð��� �ٲ��ݴϴ�.
	OwnerCharacter->SetMovable(true);
	bJumpEnd = true;
	bContinueFalling = false;
}

void UPlayerAnimInst::AnimNotify_Falling()
{	
	
	//�÷��̾ ��� �������������� üũ�մϴ�.
	bContinueFalling = true;
}

void UPlayerAnimInst::AnimNotify_CanMove()
{
	// �÷��̾��� �̵������� �������ְ� ������� ��ų�� ���������� ������� ��ų��Ͽ��� �������ݴϴ�.
	OwnerCharacter->SetMovable(true);
	OwnerCharacter->GetAttackComponent()->ResetUsingSkill();
}

void UPlayerAnimInst::AnimNotify_StopMove()
{
	//�÷��̾ ������ �� ������ �����մϴ�.
	OwnerCharacter->SetMovable(false);

	
}

void UPlayerAnimInst::AnimNotify_CanWaitSkill()
{
	// ���� �ð� ���� �÷��̾ �ٽ� ������ �����ϰԲ� �ϴ� �޼��带 ȣ���մϴ�.
	OwnerCharacter->GetAttackComponent()->SetCanWaitSkill();
}

void UPlayerAnimInst::AnimNotify_AnimEnd()
{
	// ������ ������ ������ν�ų���� ���ݴϴ�.
	OwnerCharacter->GetAttackComponent()->ResetUsingSkill();
}

void UPlayerAnimInst::AnimNotify_AttackRangeCreate()
{
	//�ִϸ��̼� ���� ������ �õ��ϴ� �������� ȣ��˴ϴ�.
	OwnerCharacter->GetAttackComponent()->TraceAttackRange();
}

void UPlayerAnimInst::AnimNotify_PlayerRespawn()
{	
	//����ִϸ��̼��� ������ ���ü��� ���ְ� ���̻� ��ȣ�ۿ��� ���� �ʵ��� �ݸ����� ����ݴϴ�.
	OwnerCharacter->GetMesh()->SetVisibility(false);
	OwnerCharacter->GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));

	// ���ü��� ������ �����ð��ڿ� �÷��̾ �������Ҽ� �ֵ��� Ÿ�̸Ӹ� �۵���ŵ�ϴ�.
	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate;
	timerDelegate.BindUObject(OwnerCharacter, &APlayerCharacter::PlayerRespawn);

	GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate ,3.0f,false);

}

void UPlayerAnimInst::AnimNotify_WalkFirst()
{	
	//�÷��̾��� ù��° �߰��� �Ҹ��� ����մϴ�.
	UGameplayStatics::PlaySound2D(GetWorld(),WalkFirstSound);
}

void UPlayerAnimInst::AnimNotify_WalkSecond()
{
	//�÷��̾��� �ι�° �߰��� �Ҹ��� ����մϴ�.
	UGameplayStatics::PlaySound2D(GetWorld(),WalkSecondSound);
}
