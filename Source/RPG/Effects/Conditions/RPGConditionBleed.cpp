// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "RPG.h"
#include "../../RPGCharacter.h"
#include "../../Components/RPGEffectManagerComponent.h"
#include "RPGConditionBleed.h"


URPGConditionBleed::URPGConditionBleed(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	ConditionType = EConditionType::Cond_Bleed;
}

void URPGConditionBleed::Tick(float DeltaTime)
{
	if (useDuration)
	{
		if (Duration >= currentDuration)
		{
			currentDuration += DeltaTime;
		}
		else
		{
			currentDuration = 0;
			AffectedTarget->EffectManager->RemoveCondition(this);
		}
	}

	if (MaxTickCount >= currentTick)
	{
		if (TickInterval >= currentTickInterval)
		{
			currentTickInterval += DeltaTime;
		}
		else
		{
			currentTick++;
			currentTickInterval = 0;
		}
	}
	else
	{
		currentTickInterval = 0;
		currentTick = 0;
	}

}

void URPGConditionBleed::ApplyCondition()
{
	Initialize();
	AffectedTarget->EffectManager->AddBleed(this);
}

UWorld* URPGConditionBleed::GetWorld() const
{
	if (AffectedTarget)
	{
		UWorld* const world = GEngine->GetWorldFromContextObject(AffectedTarget);
		return world ? world : NULL;
	}
	return NULL;
}
