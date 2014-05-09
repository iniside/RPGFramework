#include "RPG.h"
#include "../Components/RPGAttributeBaseComponent.h"
#include "RPGDamageType.h"

URPGDamageType::URPGDamageType(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
	{
	}

float URPGDamageType::DealDamage(ARPGCharacter* causer, float damageAmount)
{
	return 0;
}