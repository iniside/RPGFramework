// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "RPG.h"
#include "../Components/RPGAttributeComponent.h"
#include "../Structs/RPGSystemSructs.h"
#include "GameplayTagsManager.h"
#include "GameplayTagsModule.h"
#include "RPGEffectInstant.h"


URPGEffectInstant::URPGEffectInstant(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{

}

void URPGEffectInstant::PreInitialize()
{

}

bool URPGEffectInstant::Initialize()
{
	if (EffectApplicationType == EEffectApplicationType::Effect_ModEffect)
	{
		IsInitialized = true;
		OnEffectAppiled();
		return true;
	}
		
	Super::Initialize();
	if (Target)
	{
		OnEffectAppiled();

		OnEffectActivation.Broadcast(this->GetClass(), IsEffectActive);
		
		return true;
	}
	return false;
}

void URPGEffectInstant::Deinitialize()
{
	OnEffectEnd();
}