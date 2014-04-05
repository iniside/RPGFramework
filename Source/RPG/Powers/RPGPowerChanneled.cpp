// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "RPG.h"
//#include "../RPGWeaponBase.h"
#include "../RPGCharacter.h"
//#include "../HUD/RPGHUD.h"
#include "RPGPowerChanneled.h"


URPGPowerChanneled::URPGPowerChanneled(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
		
	
}

void URPGPowerChanneled::Tick( float DeltaTime)
{
	//Super::Tick(DeltaTime);
	channelIntervalElapsed += DeltaTime;
		if(PowerState.IsOnCooldown)
	{
		currentRechargeTime += DeltaTime;
		if(currentRechargeTime >= RechargeTime)
		{
			PowerState.IsOnCooldown = false;
			currentRechargeTime = 0;
		}
	}
	if(PowerState.IsCasting)
	{
		currentCastTime += DeltaTime;
		if(currentCastTime >= CastingTime)
		{
			PowerState.IsCasting = false;
			PowerState.IsCasted = true;
			CastPower();
			currentCastTime = 0;
		}
	}
	if(isChanneled)
	{
		if(channelIntervalElapsed >= ChannelInterval)
		{
			OnCastPower();
			channelIntervalElapsed = 0;
		}
	}
}



void URPGPowerChanneled::OnCastStart()
{
	//if(PowerOwner->EquipmentManager->MainWeapon->WeaponType == RequiredWeapon)
	//	//|| PowerOwner->EquipmentManager->OffHandWeapon->WeaponType == RequiredWeapon)
	//{
	//	if(!PowerState.IsOnCooldown)
	//	{
	//		if(PowerOwner->Attributes.Energy >= EnergyCost &&
	//			PowerOwner->Attributes.Health >= HealthCost &&
	//			PowerOwner->Attributes.Endurance >= EnduranceCost)
	//		{
	//			PowerOwner->Attributes.Energy = PowerOwner->Attributes.Energy - EnergyCost; 
	//			PowerOwner->Attributes.Health = PowerOwner->Attributes.Health - HealthCost; 
	//			PowerOwner->Attributes.Endurance = PowerOwner->Attributes.Endurance - EnduranceCost;
	//			PowerState.IsCasting = true; //reset cast time to false
	//			isChanneled = true;
	//			OnCastPower();
	//			//GetCurrentWorld()->GetTimerManager().SetTimer(this, &URPGPowerBase::OnCastPower, 1, true);
	//			//OnCastPower();
	//		}
	//	}
	//}
}

void URPGPowerChanneled::CastPower()
{
	StartChannel();
	PowerState.IsCasted = true;
	PowerState.IsCasting = false;
	PowerState.IsOnCooldown = true;
}

void URPGPowerChanneled::OnCastStop()
{
	isChanneled = false;
}

void URPGPowerChanneled::StartChannel()
{
	isChanneled = true;
}