// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "RPG.h"
#include "../Components/RPGEffectManagerComponent.h"
#include "RPGEffectHex.h"


URPGEffectHex::URPGEffectHex(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	EffectType = EEffectType::Effect_Hex;
}

void URPGEffectHex::AddEffect()
{
	Initialize();
	TargetEffectMngComp->AddEffect(this);
	TargetEffectMngComp->SetHexes(1);
}
void URPGEffectHex::SelfRemoveEffect()
{
	Deinitialize();
	TargetEffectMngComp->RemoveHexes(1);
	TargetEffectMngComp->RemoveEffect(this);
}