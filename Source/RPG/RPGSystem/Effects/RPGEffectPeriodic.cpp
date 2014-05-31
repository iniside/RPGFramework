// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "RPG.h"
#include "../Components/RPGAttributeComponent.h"
#include "GameplayTagsManager.h"
#include "GameplayTagsModule.h"
#include "RPGEffectInstant.h"
#include "RPGEffectPeriodic.h"

URPGEffectPeriodic::URPGEffectPeriodic(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{

}

void URPGEffectPeriodic::Tick(float DeltaTime)
{
	currentPeriodTime += DeltaTime;
	if (currentPeriodTime >= PeriodLenght)
	{
		OnEffectPeriodHit.Broadcast();
		OnEffectPeriod();
		currentPeriodTime = 0;
	}
	//if (ApplicationType != EApplicationType::Infinite)
	//{
	CurrentDuration += DeltaTime;
	if (CurrentDuration >= TotalDuration)
	{
		CurrentDuration = 0;
		SelfRemoveEffect();
	}
	//}
	OnEffectTick(); //this will tick every frame.
}
bool URPGEffectPeriodic::IsTickable() const
{
	//UWorld* world = GetCurrentWorld();
	if (IsEffectActive)
	{
		if (Target)
		{
			if (GetWorld())
			{
				return true;
			}
			return false;
		}
		return false;
	}
	return false;
}

TStatId URPGEffectPeriodic::GetStatId() const
{
	return this->GetStatID();
}

void URPGEffectPeriodic::PreInitialize()
{
	OnEffectPreInitialized();
}

bool URPGEffectPeriodic::Initialize()
{
	Super::Initialize();
	if (Target)
	{
		TotalDuration = PeriodLenght * PeriodsCount;

		IsEffectActive = true;

		OnEffectActivation.Broadcast(this->GetClass(), IsEffectActive);
		OnEffectAppiled();

		OnEffectInitialized();

		OnEffectPostInitialized();
		return true;
	}
	return false;
}

void URPGEffectPeriodic::Deinitialize()
{
	OnEffectDeinitialized();
	OnEffectEnd();
	IsEffectActive = false;
	PeriodLenght = 0;
	TotalDuration = 0;
}

void URPGEffectPeriodic::SelfRemoveEffect()
{
	OnEffectEnd(); //we give chance to excute any logic before effect is completly removed
	//if (ApplicationType != EApplicationType::InstantApplication)
	//{
	TargetAttributes->RemoveEffect(this);
	//}
}

void URPGEffectPeriodic::RunEffects()
{
	for (URPGEffectBase* effect : EffectsToApply)
	{
		effect->PreInitialize();
		effect->Initialize();
	}
}