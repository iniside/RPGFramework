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
					//and new duration to total pool.
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
		
		EffectsList.Add(newEffect);

}
void URPGEffectManagerComponent::RemoveEffect(class URPGEffectBase* effectToRemove)
{	
	if(effectToRemove)	
	{
		ARPGCharacter* MyPawn = Cast<ARPGCharacter>(effectToRemove->AffectedTarget);
						
		switch (effectToRemove->EffectType)			
		{				
			case EEffectType::Effect_Boon:					
				MyPawn->EffectsOnCharacter.Boons = MyPawn->EffectsOnCharacter.Boons - 1;					
				break;				
			case EEffectType::Effect_Condition:					
				MyPawn->EffectsOnCharacter.Conditions = MyPawn->EffectsOnCharacter.Conditions - 1;					
				break;				
			case EEffectType::Effect_Curse:					
				MyPawn->EffectsOnCharacter.Curses = MyPawn->EffectsOnCharacter.Curses - 1;					
				break;				
			case EEffectType::Effect_Enchantment:					
				MyPawn->EffectsOnCharacter.Enchantments = MyPawn->EffectsOnCharacter.Enchantments - 1;					
				break;				
			case EEffectType::Effect_Hex:					
				MyPawn->EffectsOnCharacter.Hexes = MyPawn->EffectsOnCharacter.Hexes - 1;					
				break;				
			default:					
				break;
		}

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

void URPGEffectManagerComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);		
}