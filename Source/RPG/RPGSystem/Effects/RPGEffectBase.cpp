// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "RPG.h"
#include "../Components/RPGAttributeComponent.h"
#include "RPGEffectBase.h"


URPGEffectBase::URPGEffectBase(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
		
	
}
void URPGEffectBase::Tick( float DeltaTime)
{
	//Super::Tick(DeltaTime);

	currentTickTime += DeltaTime;
	if(currentTickTime >= TickDuration)
	{
		if(GetWorld())
		{
			OnEffectTick();
		}
		currentTickTime = 0;
	}
	currentDuration += DeltaTime;
	if(currentDuration >= Duration)
	{
		currentDuration = 0;
		OnEffectEnd();
		SelfRemoveEffect();
	}
	OnEffectTickFrame(); //this will tick every frame.
}
bool URPGEffectBase::IsTickable() const
{
	//UWorld* world = GetCurrentWorld();
	if(IsEffectActive)
	{
		if(Target)
		{
			if(GetWorld())
			{
				//if effect is not periodic there is no point in ticking.
				if (ApplicationType == EApplicationType::Periodic)
				{
					return true;
				}
				return false;
			}
			return false;
		}
		return false;
	}
	return false;
}

TStatId URPGEffectBase::GetStatId() const
{
	return this->GetStatID();
}
//change to to bool
//so we can check if effect has been initialized properly
bool URPGEffectBase::Initialize()
{
	return false;
}

void URPGEffectBase::Deinitialize()
{
	OnEffectEnd();
	TickDuration = 0;
	Duration = 0;
	IsEffectActive = false;
}


void URPGEffectBase::AddEffect()
{
	//Initialize();
	//TargetAttributeComp->AddEffect(this);
}

void URPGEffectBase::SelfRemoveEffect()
{

}

UWorld* URPGEffectBase::GetWorld() const
{
	if (Target)
	{
		UWorld* const world = GEngine->GetWorldFromContextObject(Target);
		return world ? world : NULL;
	}
	return NULL;
}