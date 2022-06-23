// Fill out your copyright notice in the Description page of Project Settings.


#include "ZoomableSpringArm.h"

UZoomableSpringArm::UZoomableSpringArm()
{
	// 스프링암의 최대 최소 거리를 지정하고 그 중간값을 디폴트값으로 지정합니다.
	MinLength = 200.0f;
	MaxLength = 500.0f;
	TargetZoomLength = TargetArmLength = ((MaxLength + MinLength) *0.5f);

	// 줌 속도를 지정합니다.
	ZoomSpeed = 50.0f;

	bUseZoom = true;

	//마우스 회전에 따라 Y축이 변하도록 지정합니다.
	bUsePawnControlRotation = true;
}

void UZoomableSpringArm::ArmLengthSmoothChange(float deltaTime)
{
	// 스프링암이 부드럽게 전환될수 있도록 보간해줍니다.
	TargetArmLength = FMath::FInterpTo(TargetArmLength, TargetZoomLength,deltaTime, 10.0f);
}


void UZoomableSpringArm::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ArmLengthSmoothChange(DeltaTime);
}


void UZoomableSpringArm::AddArmLength(float length)
{
	if (!bUseZoom) return;
	//휠이 입력되면 목표값에 더해주고 최대 최소길이값을 넘어가지 않도록 조절합니다.
	TargetZoomLength += (length * ZoomSpeed);

	TargetZoomLength = FMath::Clamp(TargetZoomLength, MinLength, MaxLength);

}

void UZoomableSpringArm::SetUseZoom(bool useZoom)
{
	bUseZoom = useZoom;
}
