// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.?
#pragma once
#include "RPG.h"
#include "../RPGCharacter.h"
#include "RPGEffectManagerComponent.h"
#include "../RPGPlayerController.h"

URPGEffectManagerComponent::URPGEffectManagerComponent(const class FPostConstructInitializeProperties& PCIP)	
	: Super(PCIP)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void URPGEffectManagerComponent::AddEffect(URPGEffectBase* newEffect)
{
		ARPGCharacter* MyPawn = Cast<ARPGCharacter>(newEffect->AffectedTarget);
		switch (newEffect->EffectType)
		{
			case EEffectType::Effect_Boon:
				MyPawn->EffectsOnCharacter.Boons = MyPawn->EffectsOnCharacter.Boons + 1;
				//newEffect->Initialize();
				break;
			case EEffectType::Effect_Condition:
				MyPawn->EffectsOnCharacter.Conditions = MyPawn->EffectsOnCharacter.Conditions + 1;				
				//newEffect->Initialize();				
				break;			
			case EEffectType::Effect_Curse:				
				MyPawn->EffectsOnCharacter.Curses = MyPawn->EffectsOnCharacter.Curses + 1;				
				//newEffect->Initialize();				
				break;			
			case EEffectType::Effect_Enchantment:				
				MyPawn->EffectsOnCharacter.Enchantments = MyPawn->EffectsOnCharacter.Enchantments + 1;				
				//newEffect->Initialize();				
				break;			
			case EEffectType::Effect_Hex:				
				MyPawn->EffectsOnCharacter.Hexes = MyPawn->EffectsOnCharacter.Hexes + 1;				
				//newEffect->Initialize();				
				break;			
			default:				
				break;		
		}		
		EffectsList.Add(newEffect);		
		newEffect->Initialize();	

}
void URPGEffectManagerComponent::RemoveEffect(URPGEffectBase* effectToRemove)
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

void URPGEffectManagerComponent::DestroyEffect(URPGEffectBase* EffectToDestroy)
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

void URPGEffectManagerComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);		
	//ARPGCharacter* GC = Cast<ARPGCharacter>(GetOwner());	
	//check if there any effects in this component	
	//if(EffectsList.Num() > 0)	
	//{		
	//	for(URPGEffectBase* effect : EffectsList) // foreach effect in array		
	//	{			
	//		effect->TotalDuration += DeltaTime; //add delta time duration			
	//		//effect->OnEffectTicker(); //tick ticker event for effect			
	//		//effect->GetCurrentTargetHealth(effect->CurrenHealth); //send current target attributes!			
	//		if(effect->TotalDuration >= effect->Duration) //if total duration is reach			
	//		{	
	//			effect->TotalDuration = 0; //reset it to 0 (probably no need)				
	//			RemoveEffect(effect); //and remove effect from array and destroy it.				
	//			break;
	//		}		
	//	}	
	//}
}