// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "RPG.h"
//#include "../RPGWeaponBase.h"
#include "../RPGCharacter.h"
//#include "../HUD/RPGHUD.h"
#include "RPGPowerCastCharged.h"


URPGPowerCastCharged::URPGPowerCastCharged(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
		
	
}

void URPGPowerCastCharged::Tick(float DeltaTime)
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

	if (isCharged)
	{
		currentChargeTime += DeltaTime;
		if (currentChargeTime >= CastTime)
		{
			if (UseOvercharge)
			{
				if (currentChargeTime >= MaxTimeOverchargeTime)
				{
					currentChargeTime = 0.0f;
					isCharged = false;
					OnCastPower();

				}
			}
			else
			{
				currentChargeTime = 0.0f;
				isCharged = false;
				OnCastPower();
			}
		}
	}
}

void URPGPowerCastCharged::OnCastStart()
{
	isCharged = true;
	CasTimeDelegate.BindUObject(this, &URPGPowerCastCharged::GetCurrentChargeTime);
	CastPower();
}

void URPGPowerCastCharged::OnCastStop()
{
	isCharged = false;
	currentChargeTime = 0.0f;
}

void URPGPowerCastCharged::CastPower()
{

}

float URPGPowerCastCharged::GetCurrentChargeTime() { return currentChargeTime; }
//implemeny event OnCastStop to make it charged