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

	currentPeriodTime += DeltaTime;
	if (currentPeriodTime >= TickDuration)
	{
		OnEffectPeriod();
		currentPeriodTime = 0;
	}
	currentDuration += DeltaTime;
	if(currentDuration >= Duration)
	{
		currentDuration = 0;
		//OnEffectEnd();
		SelfRemoveEffect();
	}
	OnEffectTick(); //this will tick every frame.
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

void URPGEffectBase::PreInitialize()
{

}
//change to to bool
//so we can check if effect has been initialized properly
bool URPGEffectBase::Initialize()
{
	if (Target)
	{
		TArray<URPGAttributeComponent*> effectMngComps;
		URPGAttributeComponent* effectMngrComp = NULL;
		Target->GetComponents<URPGAttributeComponent>(effectMngComps);
		for (URPGAttributeComponent* effectMngComp : effectMngComps)
		{
			effectMngrComp = effectMngComp;
			break;
		}
		TArray<URPGAttributeComponent*> CausedByAttributes;
		URPGAttributeComponent* CausedByAttribute = NULL;
		CausedBy->GetComponents<URPGAttributeComponent>(CausedByAttributes);
		for (URPGAttributeComponent* effectMngComp : effectMngComps)
		{
			CausedByAttribute = effectMngComp;
			break;
		}

		if (effectMngrComp && CausedByAttribute)
		{
			TargetAttributes = effectMngrComp;
			CauserAttributes = CausedByAttribute;
			//targetAttributeComponent = effectMngrComp;
		}
		OnEffectAppiled();
		IsEffectActive = true;
		IsEffectAppiled = true;
		return true;
	}
	return false;
}

void URPGEffectBase::Deinitialize()
{
	OnEffectEnd();
	TickDuration = 0;
	Duration = 0;
	IsEffectActive = false;
}

void URPGEffectBase::SelfRemoveEffect()
{
	OnEffectEnd(); //we gice chance to excute any logic before effect is completly removed
	if (ApplicationType != EApplicationType::InstantApplication)
	{
		TargetAttributes->RemoveEffect(this);
	}
}

//void URPGEffectBase::ApplyEffectFromEffect(TSubclassOf<class URPGEffectBase> Effect)
//{
//	if (Target && TargetAttributes)
//	{
//		TargetAttributes->ApplyEffect(Target, Effect);
//	}
//}


UWorld* URPGEffectBase::GetWorld() const
{
	if (Target)
	{
		UWorld* const world = GEngine->GetWorldFromContextObject(Target);
		return world ? world : NULL;
	}
	return NULL;
}