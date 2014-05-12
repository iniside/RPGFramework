// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#include "RPG.h"
#include "../Abilities/RPGAbilityBase.h"
#include "RPGAbilityManagerComponent.h"



URPGAbilityManagerComponent::URPGAbilityManagerComponent(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{

}

void URPGAbilityManagerComponent::AddPower(TSubclassOf<URPGAbilityBase> ability, APawn* Owner)
{
	if (ability && Owner)
	{
		URPGAbilityBase* abilityObj = ConstructObject<URPGAbilityBase>(ability);

		abilityObj->Initialize(Owner, Owner->Controller);
		
		//abilityObj->SetPowerOner(MyPawn);
		//powerObj->SetMainCasterWeapon(MyPawn->EquipmentManager->MainWeapon);
		//powerObj->SetMainCasterWeapon(comp->MainWeapon);
		AbilityList.AddUnique(abilityObj);
		/*
		this should be handled by drag&drop UI
		where user just drag ability from ability inventory to hot bar

		in more elaborate scenario there can be more layers to it. Like dragging
		Spells to spell slots (to prepare them)
		and then dragging prepared spells to hotbar.
		Either way this is just debug solution that should be changed.
		*/
		if (!AbilityButtonOneObj)
		{
			AbilityButtonOneObj = abilityObj;
		}
	}
}

URPGAbilityBase* URPGAbilityManagerComponent::GetButtonOne()
{
	if (AbilityButtonOneObj)
	{
		return AbilityButtonOneObj;
	}
	return NULL;
}