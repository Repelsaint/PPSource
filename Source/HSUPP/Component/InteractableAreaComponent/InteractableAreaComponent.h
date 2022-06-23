// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "InteractableAreaComponent.generated.h"

/**
 * 
 */
 
DECLARE_DELEGATE(FOnCreateInteract)

UCLASS()
class HSUPP_API UInteractableAreaComponent : public USphereComponent
{
	GENERATED_BODY()

private:
	//�������� ��ü�� �����ڵ��Դϴ�.
	int OwnerNpcCode;

public:
	//��ȣ�ۿ��� ���۵ɶ� ȣ��� �븮���Դϴ�.
	FOnCreateInteract CreateInteractWnd;

public:
	UInteractableAreaComponent();
	virtual void BeginPlay() override;

	// ������ ���Ͱ� �������� ȣ��� �޼����Դϴ�.
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResultc);

	// ������ ���Ͱ� �������� ȣ��� �޼����Դϴ�.
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// �������� ��ü�� �ڵ带 ���ͼ� �����մϴ�.
	void SetOwnerCode(int ownerCode);

	// ���̾�α׸� �����մϴ�.
	void CreateDialog();
};
