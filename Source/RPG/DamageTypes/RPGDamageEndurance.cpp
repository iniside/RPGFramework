#include "RPG.h"
#include "../RPGCharacter.h"
#include "RPGDamageEndurance.h"

URPGDamageEndurance::URPGDamageEndurance(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
	{
	}

float URPGDamageEndurance::DealDamage(class ARPGCharacter* target, class ARPGCharacter* causer, float damageAmount)
{
	if(target)
	{
		target->SubtractCurrentEndurance(damageAmount);
		return damageAmount;
	}
	return 0;
}