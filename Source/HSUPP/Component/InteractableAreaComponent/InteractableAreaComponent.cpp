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
	// ��ȣ�ۿ� ������ ���Ͱ� ���������� ȣ��� �޼����Դϴ�.
	OnComponentBeginOverlap.AddDynamic(this,&UInteractableAreaComponent::OnOverlapBegin);
	// ��ȣ�ۿ� ������ ���Ͱ� Ż�������� ȣ��� �޼����Դϴ�.
	OnComponentEndOverlap.AddDynamic(this, &UInteractableAreaComponent::OnOverlapEnd);
}


void UInteractableAreaComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResultc)
{	
	// ������ ���� ���Ͱ� �÷��̾��� ��쿡�� �����մϴ�.
	if(Cast<APlayerCharacter>(OtherActor) == nullptr) return;
	// �÷��̾� ������ ��ȣ�ۿ밡�ɹ迭�� �� ������Ʈ�� �߰��մϴ�.
	Cast<APlayerCharacter>(OtherActor)->AddInteractableActor(this);
}

void UInteractableAreaComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// �������� Ż���� ���Ͱ� �÷��̾��� ��쿡�� �����մϴ�.
	if (Cast<APlayerCharacter>(OtherActor) == nullptr) return;
	// �÷��̾� ������ ��ȣ�ۿ밡�ɹ迭�� �� ������Ʈ�� �����մϴ�.
	Cast<APlayerCharacter>(OtherActor)->RemoveInteractableActor(this);
}

void UInteractableAreaComponent::CreateDialog()
{	
	//�� �޼��尡 ȣ��Ǿ����� ���ε� �Ǿ��ִ� �Լ��� ȣ���մϴ�.
	CreateInteractWnd.ExecuteIfBound();
}

void UInteractableAreaComponent::SetOwnerCode(int ownerCode)
{
	//�������� ��ü�� �����ڵ带 �����մϴ�.
	OwnerNpcCode = ownerCode;
}
