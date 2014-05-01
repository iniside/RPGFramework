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
	AffectedTarget->EffectManager->AddEffect(this);
	AffectedTarget->EffectManager->Hexes += 1;
	//AffectedTarget->EffectsOnCharacter.Hexes += 1;
}
void URPGEffectHex::SelfRemoveEffect()
{
	Deinitialize();
	AffectedTarget->EffectManager->Hexes -= 1;
	AffectedTarget->EffectManager->RemoveEffect(this);
}