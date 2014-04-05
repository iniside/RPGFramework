#include "RPG.h"
#include "RPGDamageType.h"

URPGDamageType::URPGDamageType(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
	{
	}

float URPGDamageType::DealDamage(ARPGCharacter* target, ARPGCharacter* causer, float damageAmount)
{
	return 0;
}