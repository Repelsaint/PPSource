// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillDamageInfo.h"

FVector FSkillDamageInfo::GetTraceDirection(AActor* ownerActor)
{
	switch (TraceDirection)
	{
	case ETraceToward::FORWARD:
		return ownerActor->GetActorForwardVector();
		break;
	case ETraceToward::BACK:
		return ownerActor->GetActorForwardVector() * -1.0f;
		break;
	case ETraceToward::RIGHT:
		return ownerActor->GetActorRightVector();
		break;
	case ETraceToward::LEFT:
		return ownerActor->GetActorRightVector() * -1.0f;
		break;
	default:
		return FVector::ZeroVector;
		break;
	}

}
