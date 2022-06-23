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
	// ��ǥ �������� ����
	float TargetZoomLength;

	//�ּ� ����
	UPROPERTY(EditAnywhere)
	float MinLength;
	//�ִ� ����
	UPROPERTY(EditAnywhere)
	float MaxLength;

	// ���� ����� �������� �ӵ�
	UPROPERTY(EditAnywhere)
		float ZoomSpeed;

		// �������� ���̸� ���������� �������� üũ�ϴ� ����
	bool bUseZoom;
		
private:

	UZoomableSpringArm();

public:
	
	// ���������� ��ġ�� �ε巴�� ��ȯ�ϱ�  ���� ȣ��Ǵ� �޼����Դϴ�.
	void ArmLengthSmoothChange(float deltaTime);

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// �������� ���̸� �߰��մϴ�.
	void AddArmLength(float length);

	// �������� ���̸� ���� ���� ���θ� �����մϴ�.
	void SetUseZoom(bool useZoom);
};
