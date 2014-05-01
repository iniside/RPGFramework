// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "RPG.h"
#include "../../RPGCharacter.h"
#include "RPGConditionBase.h"


URPGConditionBase::URPGConditionBase(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	if (TickInterval <= 0.0f || MaxTickCount <= 0.0f)
	{
		useDuration = true;
	}
	else
	{
		Duration = TickInterval * MaxTickCount;
	}
}

void URPGConditionBase::Tick(float DeltaTime)
{

}

bool URPGConditionBase::IsTickable() const
{
	if (GetWorld())
	{
		return true;
	}
	return false;
}

TStatId URPGConditionBase::GetStatId() const
{
	return this->GetStatID();
}

void URPGConditionBase::SetTarget(class ARPGCharacter* target)
{
	AffectedTarget = target;
}

void URPGConditionBase::SetAppiledBy(class ARPGCharacter* appiledBy)
{
	AppiledBy = appiledBy;
}

void URPGConditionBase::Initialize()
{

}

void URPGConditionBase::ApplyCondition()
{
	//do not do anything.
}

UWorld* URPGConditionBase::GetWorld() const
{
	if (AffectedTarget)
	{
		UWorld* const world = GEngine->GetWorldFromContextObject(AffectedTarget);
		return world ? world : NULL;
	}
	return NULL;
}
