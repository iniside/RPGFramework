// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "RPG.h"
#include "../Effects/RPGEffectBase.h"
#include "RPGAttributeComponent.h"

URPGAttributeComponent::URPGAttributeComponent(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
	PrimaryComponentTick.bCanEverTick = true;
	
}
void URPGAttributeComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URPGAttributeComponent::InitializeComponent()
{
	Super::InitializeComponent();
}
void URPGAttributeComponent::OnRegister()
{
	Super::OnRegister();
}


void URPGAttributeComponent::AddEffect(class URPGEffectBase* newEffect)
{

	int32 element = EffectsList.Find(newEffect);

	/*
	Effect is appiled instantly. We initialize it, then deinitialize
	and immidietly mark it for destruction.
	There is no reason for it to hang around and consume memory.
	*/
	if (newEffect->ApplicationType == EApplicationType::InstantApplication)
	{
		bool omg = newEffect->Initialize();
		newEffect->Deinitialize();
		DestroyEffect(newEffect);
		return;
	}

	//if effect has been found on array
	//we will check if it have special properties
	//some effects will not be appiled multiple times
	//instead they can have their duration extended
	//or can be reseted. depends on designer choices.
	if (EffectsList.Num() > 0)
	{
		//if (EffectsList[element] != nullptr)
		//{
		//	//if effect have stackable duration
		//	if (EffectsList[element]->StackDuration == true)
		//	{
		//		//we simply get duration of current effect on array
		//		//and add new duration to total pool.
		//		EffectsList[element]->Duration += newEffect->Duration;
		//		return;
		//	}

		//	/*
		//	Default behavior
		//	if the effect is on the list
		//	we remove it from it
		//	and then apply it again.
		//	This way we reset current effect, so we won't end up with multiple effects doing
		//	the same thing.
		//	*/
		//	RemoveEffect(EffectsList[element]);
		//	EffectsList.Add(newEffect);
		//}
	}
	bool success = newEffect->Initialize();
	if (success)
	{
		EffectsList.Add(newEffect);
	}

}
void URPGAttributeComponent::RemoveEffect(class URPGEffectBase* effectToRemove)
{
	if (effectToRemove)
	{
		effectToRemove->Deinitialize(); //deinitialize effect so it no longer ticks		
		int32 element = EffectsList.Find(effectToRemove);
		DestroyEffect(EffectsList[element]);
		EffectsList.RemoveSingle(effectToRemove);
	}
}

void URPGAttributeComponent::DestroyEffect(class URPGEffectBase* EffectToDestroy)
{
	if (EffectToDestroy)
	{
		if (EffectToDestroy->IsValidLowLevel())
		{
			EffectToDestroy->ConditionalBeginDestroy();
			EffectToDestroy = NULL;
		}
	}
	GetWorld()->ForceGarbageCollection(true);
}