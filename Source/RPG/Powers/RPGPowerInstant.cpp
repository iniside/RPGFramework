// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "RPG.h"
//#include "../RPGWeaponBase.h"
#include "../RPGCharacter.h"
//#include "../HUD/RPGHUD.h"
#include "../RPGPlayerController.h"
#include "../Components/RPGEquipmentManagerComponent.h"
#include "../Components/RPGEffectManagerComponent.h"
#include "RPGPowerInstant.h"


URPGPowerInstant::URPGPowerInstant(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
		
	preparationState = EPowerPreparationState::Unprepared;
}
void URPGPowerInstant::Tick(float DeltaTime)
{
	if (PowerState.IsOnCooldown)
	{
		currentRechargeTime += DeltaTime;
		if (currentRechargeTime >= RechargeTime)
		{
			PowerState.IsOnCooldown = false;
			currentRechargeTime = 0;
		}
	}
}

void URPGPowerInstant::OnCastStart()
{
	/*
	 * this block is used to determine whether the power need some preparation before cast
	 * for example display targeting circle
	 * or mark targtes for homing missle
	 * second use of power will activate it's primary effect.
	*/
	if (NeedsPreparation)
	{
		if (preparationState == EPowerPreparationState::Unprepared)
		{
			preparationState = EPowerPreparationState::Prepared;
			OnPowerPrepared();
			return;
		}
		else if (preparationState == EPowerPreparationState::Prepared)
		{
			preparationState = EPowerPreparationState::Unprepared;
		}
	}
	/*
	If I leave it like that, player will be forced to hold button to cast spell
	It might be usefull for some cases, but I also need hit button and forget behaviour.

	Oh it actually works normally. Hail the UI!
	It seems I need to special case for Charged powers (where user need to hold button
	for entire cast time duration in order to cast power.
	*/
	/*
	very basic logic to handle power usage, by player.
	More advanced system should take animation events,
	so we could just trigger this action on animation.

	I'm not sure if I should split it for spell power, mele and ranged (gun, bow etc).
	They behave bit differently than spells, but on other hand very similiar.
	Probably best solution will be testing by enum and implementing
	right logic directly in this function ;)
	*/

	if (!(PowerOwner->EquipmentManager->MainWeapon->WeaponType == RequiredWeapon))
		//|| !(PowerOwner->EquipmentManager->OffHandWeapon->WeaponType  == RequiredWeapon))
	{
		GLog->Log(FString("Main or offhand weapon can't be used with this ability!"));
		/*
		if it is on cooldown it means one of two things:
		1. Power has been recently used.
		2. It's already being used, and cooldown timer have reset. There actually needs to be
		intermadie state to btween setting cooldown and casting.
		*/
	}
	if (PowerState.IsOnCooldown)
	{
		GLogConsole->Log(FString::FormatAsNumber(currentRechargeTime));
		GLog->Log(FString("Power is still on cooldown ") + FString::FormatAsNumber(currentRechargeTime));

	}
	if (PowerState.IsCasting)
	{
		GLog->Log("Power is already casting");
		GLogConsole->Log("Power is already casting");
		//really if player want to break
		//casting, it should be possible, with some repercusions
		//but this log message will suffice for now ;)
	}

	if (PowerOwner->EquipmentManager->MainWeapon->WeaponType == RequiredWeapon)
		//|| PowerOwner->EquipmentManager->OffHandWeapon->WeaponType == RequiredWeapon)
	{
		if (!PowerState.IsOnCooldown)
		{
			if (!PowerState.IsCasting)
			{
				if (PowerOwner->GetCurrentEnergy() >= EnergyCost &&
					PowerOwner->GetCurrentHealth() >= HealthCost &&
					PowerOwner->GetCurrentEndurance() >= EnduranceCost)
				{
					PowerOwner->SubtractCurrentEnergy(EnergyCost);
					PowerOwner->SubtractCurrentHealth(HealthCost);
					PowerOwner->SubtractCurrentEndurance(EnduranceCost);
					PowerState.IsCasting = true; //reset cast time to false
					//GetCurrentWorld()->GetTimerManager().SetTimer(this, &URPGPowerBase::OnCastPower, 1, true);
					//OnCastPower();
					CastPower();
					//CasTimeDelegate.BindUObject(this, &URPGPowerBase::GetCurrentCastTime);
				}
			}
		}
	}

}

void URPGPowerInstant::CastPower()
{
	if (!PowerOwner)
		return;

	OnCastPower();
	PowerState.IsCasted = true;
	PowerState.IsCasting = false;
	PowerState.IsOnCooldown = true;
}