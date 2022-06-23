// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HSUPP.h"
#include "GameFramework/SpringArmComponent.h"
#include "ZoomableSpringArm.generated.h"

/**
 * 
 */
UCLASS()
class HSUPP_API UZoomableSpringArm : public USpringArmComponent
{
	GENERATED_BODY()



private:
	// 목표 스프링암 길이
	float TargetZoomLength;

	//최소 길이
	UPROPERTY(EditAnywhere)
	float MinLength;
	//최대 길이
	UPROPERTY(EditAnywhere)
	float MaxLength;

	// 길이 변경시 스프링암 속도
	UPROPERTY(EditAnywhere)
		float ZoomSpeed;

		// 스프링암 길이를 조정가능한 상태인지 체크하는 변수
	bool bUseZoom;
		
private:

	UZoomableSpringArm();

public:
	
	// 스프링암의 위치를 부드럽게 변환하기  위해 호출되는 메서드입니다.
	void ArmLengthSmoothChange(float deltaTime);

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// 스프링암 길이를 추가합니다.
	void AddArmLength(float length);

	// 스프링암 길이를 변경 가능 여부를 설정합니다.
	void SetUseZoom(bool useZoom);
};
