// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "RPG.h"
#include "RPGAttributeBaseComponent.h"

URPGAttributeBaseComponent::URPGAttributeBaseComponent(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}
void URPGAttributeBaseComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
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
	if (CanRegenerateEnergy)
	{
		if (Energy < GetMaxEnergy())
		{
			currentEnergyRegenInterval += DeltaTime;
			if (currentHealthRegenInterval >= EnergyRegenInterval)
			{
				Energy += EnergyRegenValue;
				currentEnergyRegenInterval = 0;
			}
		}
	}
	if (CanRegenerateStamina)
	{
		if (Energy < GetMaxStamina())
		{
			currentStaminaRegenInterval += DeltaTime;
			if (currentStaminaRegenInterval >= StaminaRegenInterval)
			{
				Stamina += StaminaRegenValue;
				currentStaminaRegenInterval = 0;
			}
		}
	}
}
void URPGAttributeBaseComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetConsitutionMod();
	SetIntelligenceMod();
	SetEnduranceMod();
	Health = GetMaxHealth();
	Energy = GetMaxEnergy();
	Stamina = GetMaxStamina();
}
void URPGAttributeBaseComponent::OnRegister()
{
	Super::OnRegister();
	SetConsitutionMod();
	SetIntelligenceMod();
	SetEnduranceMod();
	SetMaxHealth();
	SetMaxEnergy();
	SetMaxStamina();
	SetSpellCritChance();
}
void URPGAttributeBaseComponent::PostInitProperties()
{
	Super::PostInitProperties();
}
