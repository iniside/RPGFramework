// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.?
#pragma once
#include "RPG.h"
#include "../RPGCharacter.h"
#include "../Effects/RPGEffectBase.h"
#include "../Effects/Conditions/RPGConditionBase.h"
#include "../Effects/Conditions/RPGConditionBleed.h"
#include "RPGEffectManagerComponent.h"
#include "../RPGPlayerController.h"

URPGEffectManagerComponent::URPGEffectManagerComponent(const class FPostConstructInitializeProperties& PCIP)	
	: Super(PCIP)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void URPGEffectManagerComponent::AddEffect(class URPGEffectBase* newEffect)
{
		ARPGCharacter* MyPawn = Cast<ARPGCharacter>(newEffect->AffectedTarget);

		int32 element = EffectsList.Find(newEffect);

		//if effect has been found on array
		//we will check if it have special properties
		//some effects will not be appiled multiple times
		//instead they can have their duration extended
		//or can be reseted. depends on designer choices.
		
		if (EffectsList.Num() > 0)
		{
			if (EffectsList[element] != nullptr)
			{
				//if effect have stackable duration
				if (EffectsList[element]->DurationStackable == true)
				{
					//we simply get duration of current effect on array
					//and add new duration to total pool.
					EffectsList[element]->Duration += newEffect->Duration;
					return;
				}

				/*
				Default behavior
				if the effect is on the list
				we remove it from it
				and then apply it again.
				This way we reset current effect, so we won't end up with multiple effects doing
				the same thing.
				*/
				RemoveEffect(EffectsList[element]);
				EffectsList.Add(newEffect);
			}
		}
		bool success = newEffect->Initialize();
		if (success)
		{
			EffectsList.Add(newEffect);
		}

}
void URPGEffectManagerComponent::RemoveEffect(class URPGEffectBase* effectToRemove)
{	
	if(effectToRemove)	
	{
		effectToRemove->Deinitialize(); //deinitialize effect so it no longer ticks		
		int32 element = EffectsList.Find(effectToRemove);		
		DestroyEffect(EffectsList[element]);		
		EffectsList.RemoveSingle(effectToRemove);	
	}
}

void URPGEffectManagerComponent::DestroyEffect(class URPGEffectBase* EffectToDestroy)
{	
	if(EffectToDestroy)	
	{		
		if(EffectToDestroy->IsValidLowLevel())		
		{				
			EffectToDestroy->ConditionalBeginDestroy();				
			EffectToDestroy = NULL;		
		}	
	}	
	GetWorld()->ForceGarbageCollection(true);
}
void URPGEffectManagerComponent::AddCondition(URPGConditionBase* condition)
{

}
void URPGEffectManagerComponent::AddBleed(class URPGConditionBleed* condition)
{
	ConditionBleeds.Add(condition);
}
void URPGEffectManagerComponent::RemoveBleed(class URPGConditionBleed* condition)
{
	int32 elements = ConditionBleeds.Remove(condition);
	DestroyCondition(condition);
}
void URPGEffectManagerComponent::RemoveCondition(class URPGConditionBase* condition)
{
}

void URPGEffectManagerComponent::DestroyCondition(class URPGConditionBase* condition)
{
	if (condition)
	{
		if (condition->IsValidLowLevel())
		{
			condition->ConditionalBeginDestroy();
			condition = NULL;
		}
	}
	GetWorld()->ForceGarbageCollection(true);
}

uint8 URPGEffectManagerComponent::GetHexes()
{
	return Hexes;
}
void URPGEffectManagerComponent::SetHexes(uint8 amount)
{
	Hexes += amount;
}
void URPGEffectManagerComponent::RemoveHexes(uint8 amount)
{
	Hexes -= amount;
}

uint8 URPGEffectManagerComponent::GetCurses()
{
	return Curses;
}
void URPGEffectManagerComponent::SetCurses(uint8 amount)
{
	Curses += amount;
}
void URPGEffectManagerComponent::RemoveCurses(uint8 amount)
{
	Curses -= amount;
}

uint8 URPGEffectManagerComponent::GetEnchantments()
{
	return Enchantments;
}
void URPGEffectManagerComponent::SetEnchantments(uint8 amount)
{
	Enchantments += amount;
}
void URPGEffectManagerComponent::RemoveEnchantments(uint8 amount)
{
	Enchantments -= amount;
}

uint8 URPGEffectManagerComponent::GetConditions()
{
	return Conditions;
}
void URPGEffectManagerComponent::SetConditions(uint8 amount)
{
	Conditions += amount;
}
void URPGEffectManagerComponent::RemoveConditions(uint8 amount)
{
	Conditions -= amount;
}

uint8 URPGEffectManagerComponent::GetBoons()
{
	return Boons;
}
void URPGEffectManagerComponent::SetBoons(uint8 amount)
{
	Boons += amount;
}
void URPGEffectManagerComponent::RemoveBoons(uint8 amount)
{
	Boons -= amount;
}
void URPGEffectManagerComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);		
}