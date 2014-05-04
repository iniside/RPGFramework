#include "RPG.h"
#include "../Components/RPGAttributeComponent.h"
#include "RPGDamageType.h"

URPGDamageType::URPGDamageType(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
	{
	}

float URPGDamageType::DealDamage(class URPGAttributeComponent* target, ARPGCharacter* causer, float damageAmount)
{
	return 0;
}