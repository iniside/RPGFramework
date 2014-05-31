// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "RPG.h"
#include "../Components/RPGAttributeComponent.h"
#include "../Structs/RPGSystemSructs.h"
#include "GameplayTagsManager.h"
#include "GameplayTagsModule.h"
#include "RPGEffectCombat.h"


URPGEffectCombat::URPGEffectCombat(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{

}

void URPGEffectCombat::PreInitialize()
{

}


//Modifier effect is always initialized.
//Because it's only task is to modify other effects, it doesn't care about actors.
bool URPGEffectCombat::Initialize()
{
	return Super::Initialize();
}

void URPGEffectCombat::Deinitialize()
{
	OnEffectEnd();
}