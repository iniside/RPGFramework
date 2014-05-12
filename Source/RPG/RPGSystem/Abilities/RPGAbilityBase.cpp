// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "RPG.h"
#include "../Components/RPGAttributeComponent.h"
#include "RPGAbilityBase.h"


URPGAbilityBase::URPGAbilityBase(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	isOnCooldown = false;
	isChanneled = false;
	isCasted = false;
}

void URPGAbilityBase::Tick(float DeltaTime)
{
	if (isOnCooldown)
	{
		currentCooldownTime += DeltaTime;
		if (currentCooldownTime >= AbilityCooldownTime)
		{
			currentCooldownTime = 0;
			isOnCooldown = false;
		}
	}

	if (isChanneled)
	{
		currentIntervalTime += DeltaTime;
		if (currentIntervalTime >= ChannelInterval)
		{
			currentIntervalTime = 0;
			OnAbilityStart();
		}
	}

	if (isCasted)
	{
		currentCastTime += DeltaTime;
		if (currentCastTime >= MaxCastTime)
		{
			currentCastTime = 0;
			isCasted = false;
			OnAbilityStart();
		}
	}
	if (isCharged)
	{
		currentChargeTime += DeltaTime;
		if (currentChargeTime >= MaxCastTime)
		{
			currentChargeTime = 0;
			isCharged = false;
			OnAbilityStart();
		}
	}
}

bool URPGAbilityBase::IsTickable() const
{
	if (AbilityOwner)
	{
		if (GetWorld())
		{
			return true;
		}
		return false;
	}
	return false;
}
TStatId URPGAbilityBase::GetStatId() const
{
	return this->GetStatID();
}

UWorld* URPGAbilityBase::GetWorld() const
{
	if (!AbilityOwner)
		return NULL;
	UWorld* const World = GEngine->GetWorldFromContextObject(AbilityOwner);
	return World ? World : nullptr;
}

/*
	If there is no component on actor then we don't initialize
	because powers need attributes to work properly.
*/
void URPGAbilityBase::Initialize(APawn* owner, AController* instigator)
{
	AbilityOwner = owner;
	Instigator = instigator;

	if (AbilityOwner)
	{
		if (GetWorld())
		{
			TArray<URPGAttributeComponent*> attributeComps;
			AbilityOwner->GetComponents<URPGAttributeComponent>(attributeComps);
			for (URPGAttributeComponent* attrComp : attributeComps)
			{
				OwnerAttributeComp = attrComp;
				break;
			}
			if (OwnerAttributeComp)
			{
				IsAbilityInitialized = true;
				return;
			}
			IsAbilityInitialized = false;
		}
		IsAbilityInitialized =  false;
	}
	IsAbilityInitialized =  false;
}

void URPGAbilityBase::InputPressed()
{
	bool haveRequiredWeapon = false;
	if (IsAbilityInitialized)
	{
		/*
		in reality we need more sophisitacted cooldown handling. If user for example
		cancel ability withing first 0.75 seconds cooldown will be shorter.
		*/
		if (!isOnCooldown)
		{
			if (AbilityCastType == ECastType::Casted)
			{
				isCasted = true;
				isOnCooldown = true;
			}
			if (AbilityCastType == ECastType::Casted_Charged)
			{
				isCharged = true;
				isOnCooldown = true;
			}
			if (AbilityCastType == ECastType::Channeled)
			{
				OnAbilityStart();
				isChanneled = true;
				isOnCooldown = true;
			}
		}
	}
}

void URPGAbilityBase::InputReleased()
{
	if (AbilityCastType == ECastType::Channeled)
	{
		OnAbilityStop();
		isChanneled = false;
	}
}

