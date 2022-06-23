// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInst.h"
#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"

UPlayerAnimInst::UPlayerAnimInst()
{	
	// 캐릭터가 이동할 때 낼 사운드를 호출합니다. 왼쪽발과 오른쪽발의 발소리가 조금 다르게 하기 위해서 각각 따로 불러들입니다.
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

	// 소유중인캐릭터를 지정합니다.
	OwnerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
}

void UPlayerAnimInst::NativeUpdateAnimation(float dt)
{
	Super::NativeUpdateAnimation(dt);
	
	// 만약 소유중인 플레이어가 없다면 실행하지 않습니다.
	if (!IsValid(OwnerCharacter)) return;
	
	//기존에 플레이어의 속도에 맞춰서 애니메이션이 재생되게 하였으나
	// wasd의 잦은 입력으로 캐릭터가 방향을 빠르게 바꾸게 되면
	// 플레이어가 움직이는 애니메이션 속도가 정지상태에 이르기때문에 강제로 이동속도를 고정합니다.
	//MoveSpeed = OwnerCharacter->GetVelocity().Size();
	if (OwnerCharacter->GetVelocity().Size() > 0.0f && OwnerCharacter->GetVelocity().Size() <= 501.0f)
	{
		MoveSpeed = 500.0f;
	}
	else if (OwnerCharacter->GetVelocity().Size() <= 1000.0f && OwnerCharacter->GetVelocity().Size() > 501.0f)
		MoveSpeed = 1000.0f;
	else if (OwnerCharacter->GetVelocity().Size() == 0.0f)
		MoveSpeed = 0.0f;
	
	
	
	// 플레이어의 무브컴포넌트를 통해 플레이어가 공중인지 아닌지를 체크합니다.
	bIsInAir = OwnerCharacter->GetMovementComponent()->IsFalling();
	
	// 만약 공중이라면 움직일수 없도록 하고 점프가 끝나지 않았다는것을 체크를 해줍니다.	
	if (bIsInAir)
	{
		OwnerCharacter->SetMovable(false);
		bJumpEnd = false;
	}
}

void UPlayerAnimInst::AnimNotify_JumpEnd()
{
	// 땅에 닿으면 플레이어가 움직일수 있도록 셋팅값을 바꿔줍니다.
	OwnerCharacter->SetMovable(true);
	bJumpEnd = true;
	bContinueFalling = false;
}

void UPlayerAnimInst::AnimNotify_Falling()
{	
	
	//플레이어가 계속 떨어지고있음을 체크합니다.
	bContinueFalling = true;
}

void UPlayerAnimInst::AnimNotify_CanMove()
{
	// 플레이어의 이동가능을 지정해주고 사용중인 스킬이 끝났음으로 사용중인 스킬목록에서 제외해줍니다.
	OwnerCharacter->SetMovable(true);
	OwnerCharacter->GetAttackComponent()->ResetUsingSkill();
}

void UPlayerAnimInst::AnimNotify_StopMove()
{
	//플레이어가 움직일 수 없도록 지정합니다.
	OwnerCharacter->SetMovable(false);

	
}

void UPlayerAnimInst::AnimNotify_CanWaitSkill()
{
	// 일정 시간 이후 플레이어가 다시 공격이 가능하게끔 하는 메서드를 호출합니다.
	OwnerCharacter->GetAttackComponent()->SetCanWaitSkill();
}

void UPlayerAnimInst::AnimNotify_AnimEnd()
{
	// 공격이 끝나면 사용중인스킬에서 빼줍니다.
	OwnerCharacter->GetAttackComponent()->ResetUsingSkill();
}

void UPlayerAnimInst::AnimNotify_AttackRangeCreate()
{
	//애니메이션 도중 공격을 시도하는 지점에서 호출됩니다.
	OwnerCharacter->GetAttackComponent()->TraceAttackRange();
}

void UPlayerAnimInst::AnimNotify_PlayerRespawn()
{	
	//사망애니메이션이 끝난후 가시성을 꺼주고 더이상 상호작용이 되지 않도록 콜리전을 비워줍니다.
	OwnerCharacter->GetMesh()->SetVisibility(false);
	OwnerCharacter->GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));

	// 가시성을 꺼준후 일정시간뒤에 플레이어를 리스폰할수 있도록 타이머를 작동시킵니다.
	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate;
	timerDelegate.BindUObject(OwnerCharacter, &APlayerCharacter::PlayerRespawn);

	GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate ,3.0f,false);

}

void UPlayerAnimInst::AnimNotify_WalkFirst()
{	
	//플레이어의 첫번째 발걸음 소리를 출력합니다.
	UGameplayStatics::PlaySound2D(GetWorld(),WalkFirstSound);
}

void UPlayerAnimInst::AnimNotify_WalkSecond()
{
	//플레이어의 두번째 발걸음 소리를 출력합니다.
	UGameplayStatics::PlaySound2D(GetWorld(),WalkSecondSound);
}
