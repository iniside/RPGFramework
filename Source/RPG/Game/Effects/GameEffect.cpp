// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "RPG.h"
#include "../Components/RPGAttributeBaseComponent.h"
#include "GameEffect.h"


UGameEffect::UGameEffect(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
		
	
}
void UGameEffect::Tick( float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetTargetAttributeUpdate();
}
void UGameEffect::PreInitialize()
{
	switch (EffectType)
	{
		case EEffectType::Hex:
			OwnedTags.AddTag("Hex");
		default:
			break;
	}
}
bool UGameEffect::Initialize()
{
	return Super::Initialize();
	//IsEffectActive = false;
	//if (Target)
	//{
	//	if (GetWorld())
	//	{
	//		TArray<URPGAttributeBaseComponent*> attributeComps;
	//		Target->GetComponents<URPGAttributeBaseComponent>(attributeComps);
	//		for (URPGAttributeBaseComponent* attrComp : attributeComps)
	//		{
	//			TargetAttributes = attrComp;
	//			break;
	//		}
	//		TArray<URPGAttributeBaseComponent*> causedByAttributeComps;
	//		CausedBy->GetComponents<URPGAttributeBaseComponent>(causedByAttributeComps);
	//		for (URPGAttributeBaseComponent* attrComp : causedByAttributeComps)
	//		{
	//			CauserAttributes = attrComp;
	//			break;
	//		}
	//		if (TargetAttributes)
	//		{
	//			OnEffectAppiled();
	//			IsEffectActive = true;
	//			return true;
	//		}
	//		return false;
	//	}
	//	return false;
	//}
	//return false;
}

void UGameEffect::SetTargetAttributeUpdate()
{
	//TargetHealth = TargetAttributes->GetHealth();
	//TargetEnergy = TargetAttributes->GetEnergy();
	//TargetStamina = TargetAttributes->GetStamina();
}