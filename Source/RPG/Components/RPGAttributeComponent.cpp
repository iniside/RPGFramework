// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "RPG.h"
#include "../RPGCharacter.h"
//#include "../HUD/RPGHUD.h"
//#include "../HUD/Widgets/SRPGPowerInventoryWidget.h"
#include "RPGAttributesBase.h"
#include "../DamageTypes/RPGDamageType.h"
#include "RPGAttributeComponent.h"
#include "../RPGPlayerController.h"


URPGAttributeComponent::URPGAttributeComponent(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
	PrimaryComponentTick.bCanEverTick = true;
	MaxHealth = Constitution * 100;
}
void URPGAttributeComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CanRegenerateHealth)
	{
		if (Health < GetMaxHealth())
		{
			currentHealthRegenInterval += DeltaTime;
			if (currentHealthRegenInterval >= HealthRegenInterval)
			{
				Health += HealthRegenValue;
				currentHealthRegenInterval = 0;
			}
		}
	}
}

void URPGAttributeComponent::ChanageAttribute(TEnumAsByte<EAttributeChange> ChangeType, int16 value)
{
	//this is just mockup
	switch (ChangeType)
	{
		case EAttributeChange::Add:
		{
			Health += value;
			OnAttributeChanged.Broadcast();
			break;
		}
		default:
			break;
	}
}

void URPGAttributeComponent::TakeDamage(float Damage, TSubclassOf<URPGDamageType> DamageType)
{
	URPGDamageType* damage = ConstructObject<URPGDamageType>(DamageType);
	UProperty* HealthProperty = FindFieldChecked<UProperty>(URPGAttributesBase::StaticClass(), GET_MEMBER_NAME_CHECKED(URPGAttributesBase, HealthProp));
	
	damage->DealDamage(this, NULL, Damage);
}