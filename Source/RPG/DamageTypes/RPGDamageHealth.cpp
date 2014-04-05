#include "RPG.h"
#include "../RPGCharacter.h"
#include "RPGDamageHealth.h"

URPGDamageHealth::URPGDamageHealth(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
	{
	}

float URPGDamageHealth::DealDamage(ARPGCharacter* target, ARPGCharacter* causer, float damageAmount)
{
	if(target)
	{
		float damage;
		/*
		so final equation should look differently.
		first we should calculate damage, by using any modifiers to the damageAmount input, we see fit.
		and then subtract health from character (or anything else for that matter)
		*/
		damage = damageAmount;
		target->SubtractCurrentHealth(damage);
		/*
		this is very ugly hack, so we can pass how much damage we have dealth to target back to the caster.
		It should be moved into data structure. Possibily accessed trough getter/setter
		but for now it should work.
		*/
		//causer->DamageCaused = damage;
		/*
		We could also use data structure to pass to causer, health of target, to display on UI. It should be probably Array of simple data struct.
		*/
		return damage;
	}
	return 0;
}