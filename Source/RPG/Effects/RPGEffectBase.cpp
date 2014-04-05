// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "RPG.h"
#include "../Common/RPGSharedEnums.h"
#include "../Components/RPGEffectManagerComponent.h"
#include "RPGEffectBase.h"


URPGEffectBase::URPGEffectBase(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
		
	
}
void URPGEffectBase::Tick( float DeltaTime)
{
	//Super::Tick(DeltaTime);
	if(!AffectedTarget)
		return;
	if(!GetWorld())
		return;
	if(!IsEffectActive)
		return;

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
		AffectedTarget->EffectManager->RemoveEffect(this);
	}
	OnEffectTickFrame(); //this will tick every frame.
	SetAttributeUpdates();
}
bool URPGEffectBase::IsTickable() const
{
	//UWorld* world = GetCurrentWorld();
	if(IsEffectActive)
	{
		if(AffectedTarget)
		{
			if(GetWorld())
			{
				return true;
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

void URPGEffectBase::SetAttributeUpdates()
{
	TargetHealth = AffectedTarget->GetCurrentHealth();
	//TargetHealthPrecentage = AffectedTarget->GetHealthPrecentage();
	TargetEnergy = AffectedTarget->GetCurrentEnergy();
	TargetEndurance = AffectedTarget->GetCurrentEndurance();

	CauserHealth = CausedBy->GetCurrentHealth();
	CauserEnergy = CausedBy->GetCurrentEnergy();
	CauserEndurance = CausedBy->GetCurrentEndurance();
}

bool URPGEffectBase::CheckHealthTreshold(float threshold)
{
	return false;
}
void URPGEffectBase::Initialize()
{
	IsEffectActive = false;
	EffectsRemoved = 0;
	if(AffectedTarget)
	{
		UWorld* world = GetCurrentWorld(AffectedTarget);
		if(CurrentWorld)
		{
			//OnEffectAppiled_Implementation();
			OnEffectAppiled();
			IsEffectActive = true;
		}
	}
}

void URPGEffectBase::Deinitialize()
{
	OnEffectEnd();
	EffectsRemoved = 0;
	TickDuration = 0;
	Duration = 0;
	IsEffectActive = false;
}

void URPGEffectBase::OnEffectAppiled_Implementation()
{
}
//
//void URPGEffectBase::OnEffectTick_Implementation()
//{
//}
//
//void URPGEffectBase::OnValueChanged_Implementation()
//{
//}
//
//void URPGEffectBase::OnEffectRemoved_Implementation()
//{
//	//ARPGCharacter* GC = Cast<ARPGCharacter>(AffectedTarget);
//	//if(GC)
//	//{
//	//	UWorld* world = GetCurrentWorld(GC);
//	//	if(world)
//	//	{
//	//		FTimerDynamicDelegate tdd;
//	//		tdd.BindDynamic(this, &URPGEffectBase::RemoveFromArray);
//	//		float currentTime = world->GetTimerManager().GetTimerElapsed(tdd);
//	//		if(currentTime >= Duration)
//	//		{
//	//			timer = true;
//	//		}
//	//		else
//	//		{
//	//			timer = false;
//	//		}
//	//	}
//	//}
//}

void URPGEffectBase::OnEffectTickFrame_Implementation()
{

}

void URPGEffectBase::SetCurrentWorld(UWorld* world)
{
	if(world)
	{
		CurrentWorld = world;
	}
}


void URPGEffectBase::RemoveSingleEffect(TEnumAsByte<EEffectType> appiledEffectType)
{
	////int32 effectCount = 0; //actually it could be stored in protected property. Let's see!
	//int32 arrayNum = AffectedTarget->EffectManager->EffectsList.Num(); //just checkin remove it!
	//if(AffectedTarget)
	//{
	//	//use iterator. Array will dynamically change, as elements are removed in this loop (!!).
	//	//They are also removed outside of loop, when they expire.
	//	//Iterator will take care for it, for us.
	//	for(auto it = AffectedTarget->EffectManager->EffectsList.CreateIterator(); it; ++it)
	//	{
	//		URPGEffectBase* effect  = AffectedTarget->EffectManager->EffectsList[it.GetIndex()];
	//		if(effect->EffectType == appiledEffectType)
	//		{
	//			AffectedTarget->EffectManager->RemoveEffect(effect); 
	//			EffectsRemoved++; //increment effects removed by this effect
	//			return;// effectCount = 1; // return we only looking for first match.
	//		}
	//	}
	//	//return effectCount;
	//}
	////return effectCount;
}

UWorld* URPGEffectBase::GetWorld() const
{
	UWorld* const world = GEngine->GetWorldFromContextObject(CausedBy);
	return world ? world : NULL;
}
UWorld* URPGEffectBase::GetCurrentWorld(UObject* worldContext)
{
	UWorld* const World = GEngine->GetWorldFromContextObject(worldContext);
    return World ? World : NULL;
}