// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableAreaComponent.h"
#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"
#include "Widget/NpcDialog/NpcDialog.h"

UInteractableAreaComponent::UInteractableAreaComponent()
{
}

void UInteractableAreaComponent::BeginPlay()
{
	Super::BeginPlay();
	// 상호작용 영역에 액터가 진입했을때 호출될 메서드입니다.
	OnComponentBeginOverlap.AddDynamic(this,&UInteractableAreaComponent::OnOverlapBegin);
	// 상호작용 영역에 액터가 탈출했을때 호출될 메서드입니다.
	OnComponentEndOverlap.AddDynamic(this, &UInteractableAreaComponent::OnOverlapEnd);
}


void UInteractableAreaComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResultc)
{	
	// 영역에 들어온 액터가 플레이어인 경우에만 실행합니다.
	if(Cast<APlayerCharacter>(OtherActor) == nullptr) return;
	// 플레이어 액터의 상호작용가능배열에 이 컴포넌트를 추가합니다.
	Cast<APlayerCharacter>(OtherActor)->AddInteractableActor(this);
}

void UInteractableAreaComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 영역에서 탈출한 액터가 플레이어인 경우에만 실행합니다.
	if (Cast<APlayerCharacter>(OtherActor) == nullptr) return;
	// 플레이어 액터의 상호작용가능배열에 이 컴포넌트를 제거합니다.
	Cast<APlayerCharacter>(OtherActor)->RemoveInteractableActor(this);
}

void UInteractableAreaComponent::CreateDialog()
{	
	//이 메서드가 호출되었을때 바인딩 되어있는 함수를 호출합니다.
	CreateInteractWnd.ExecuteIfBound();
}

void UInteractableAreaComponent::SetOwnerCode(int ownerCode)
{
	//소유중인 객체의 고유코드를 설정합니다.
	OwnerNpcCode = ownerCode;
}
