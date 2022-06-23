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
	//소유중인 객체의 고유코드입니다.
	int OwnerNpcCode;

public:
	//상호작용이 시작될때 호출될 대리자입니다.
	FOnCreateInteract CreateInteractWnd;

public:
	UInteractableAreaComponent();
	virtual void BeginPlay() override;

	// 영역에 액터가 들어왔을떄 호출될 메서드입니다.
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResultc);

	// 영역에 액터가 나갔을때 호출될 메서드입니다.
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// 소유중인 객체의 코드를 얻어와서 설정합니다.
	void SetOwnerCode(int ownerCode);

	// 다이얼로그를 생성합니다.
	void CreateDialog();
};
