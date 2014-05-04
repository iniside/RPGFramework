// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "RPG.h"
#include "../Common/RPGSharedEnums.h"
#include "../Components/RPGEffectManagerComponent.h"
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
		RestoreOriginalAttributes(AttributeDecrease);
		RestoreOriginalMaxHealth(MaxHealthDrain);
		currentDuration = 0;
		OnEffectEnd();
		SelfRemoveEffect();
		//AffectedTarget->EffectManager->RemoveEffect(this);
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
//change to to bool
//so we can check if effect has been initialized properly
bool URPGEffectBase::Initialize()
{
	IsEffectActive = false;
	EffectsRemoved = 0;
	if (AffectedTarget)
	{
		if (GetWorld())
		{
			TArray<URPGAttributeComponent*> attributeComps;
			AffectedTarget->GetComponents<URPGAttributeComponent>(attributeComps);
			for (auto attrComp : attributeComps)
			{
				TargetAttributeComp = attrComp;
				break;
			}
			
			TArray<URPGEffectManagerComponent*> effectMngComps;
			AffectedTarget->GetComponents<URPGEffectManagerComponent>(effectMngComps);
			for (auto effectMngComp : effectMngComps)
			{
				TargetEffectMngComp = effectMngComp;
				break;
			}
			if (TargetAttributeComp && TargetEffectMngComp)
			{
				//OnEffectAppiled_Implementation();
				OnEffectAppiled();
				IsEffectActive = true;
				return true;
			}
			return false;
		}
		return false;
	}
	return false;
}

void URPGEffectBase::Deinitialize()
{
	OnEffectEnd();
	EffectsRemoved = 0;
	TickDuration = 0;
	Duration = 0;
	IsEffectActive = false;
	TargetAttributeComp = NULL;
	TargetEffectMngComp = NULL;
}
void URPGEffectBase::SetAttributeUpdates()
{
	TargetHealth = TargetAttributeComp->GetHealth();
	//TargetHealthPrecentage = AffectedTarget->GetHealthPrecentage();
	//TargetEnergy = AffectedTarget->GetCurrentEnergy();
	//TargetEndurance = AffectedTarget->GetCurrentEndurance();

	//CauserHealth = CausedBy->GetCurrentHealth();
	//CauserEnergy = CausedBy->GetCurrentEnergy();
	//CauserEndurance = CausedBy->GetCurrentEndurance();
}

bool URPGEffectBase::CheckHealthTresholdMaxHealth(float threshold)
{
	float healthPrecentage = TargetHealth / TargetAttributeComp->GetHealth() * 100.0f;
	if (healthPrecentage > threshold)
	{
		return false;
	}
	else
	{
		true;
	}
	return false;
}

void URPGEffectBase::AddEffect()
{
	//do not do anything.
}


void URPGEffectBase::OnEffectAppiled_Implementation()
{
}

void URPGEffectBase::OnEffectTickFrame_Implementation()
{
	
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
void URPGEffectBase::DecreaseAttributeForTime(float DecreaseValue)
{
	if (AffectedTarget)
	{
		//OriginalConstitution = AffectedTarget->BaseAttributes.Constitution;
		AttributeDecrease = DecreaseValue;
		//AffectedTarget->BaseAttributes.Constitution -= DecreaseValue;
	}
}
void URPGEffectBase::RestoreOriginalAttributes(float DecreasedValue)
{
	if (AffectedTarget)
	{
		//AffectedTarget->BaseAttributes.Constitution += DecreasedValue;
	}
}
void URPGEffectBase::TemporarlyDrainHealth(float DrainAmount)
{

}
void URPGEffectBase::RestoreOriginalHealth(float HealthValue)
{

}

void URPGEffectBase::TemporarlyDrainMaxHealth(float DrainAmount)
{
	if (AffectedTarget)
	{
		MaxHealthDrain = DrainAmount;
		//AffectedTarget->SubtractMaxHealth(DrainAmount);
	}
}
void URPGEffectBase::RestoreOriginalMaxHealth(float HealthValue)
{
	if (AffectedTarget)
	{
		//AffectedTarget->AddMaxHealth(HealthValue);
	}
}

uint8 URPGEffectBase::GetHexesFromTarget()
{
	return TargetEffectMngComp->GetHexes();
}
uint8 URPGEffectBase::GetCursesFromTarget()
{
	return TargetEffectMngComp->GetCurses();
}
uint8 URPGEffectBase::GetEnchantmentsFromTarget()
{
	return TargetEffectMngComp->GetEnchantments();
}
uint8 URPGEffectBase::GetConditionsFromTarget()
{
	return TargetEffectMngComp->GetConditions();
}
uint8 URPGEffectBase::GetBoonsFromTarget()
{
	return TargetEffectMngComp->GetBoons();
}
void URPGEffectBase::SelfRemoveEffect()
{

}

UWorld* URPGEffectBase::GetWorld() const
{
	if (AffectedTarget)
	{
		UWorld* const world = GEngine->GetWorldFromContextObject(AffectedTarget);
		return world ? world : NULL;
	}
	return NULL;
}