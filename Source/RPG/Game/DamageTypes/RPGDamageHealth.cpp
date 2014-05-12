#include "RPG.h"
#include "../RPGCharacter.h"
#include "../Components/RPGAttributeBaseComponent.h"
#include "RPGDamageHealth.h"

URPGDamageHealth::URPGDamageHealth(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
	{
	}

float URPGDamageHealth::DealDamage(ARPGCharacter* causer, float damageAmount)
{
	if(DamageTarget)
	{
		/*
		so final equation should look differently.
		first we should calculate damage, by using any modifiers to the damageAmount input, we see fit.
		and then subtract health from character (or anything else for that matter)
		*/
		//if (DamageCauser->SpellCritted())
		//{
		//	DamageTarget->SubtractHealth(damageAmount*2);
		//	return 0;
		//}
		//DamageTarget->SubtractHealth(damageAmount);
		/*
		this is very ugly hack, so we can pass how much damage we have dealth to target back to the caster.
		It should be moved into data structure. Possibily accessed trough getter/setter
		but for now it should work.
		*/
		//causer->DamageCaused = damage;
		/*
		We could also use data structure to pass to causer, health of target, to display on UI. It should be probably Array of simple data struct.
		*/
		return damageAmount;
	}
	return 0;
}

/*
this is pseudocode for calculating spellDamage;
DamageType_Spell(AttributeComponent* attribute, Blueprint)
{
//order in which calculations are made is crucial!

int32 finalDamage = Blueprint * attribute->SpellDamage;
if(rand() < attribute->CritChange)
{
finalDamage *= CritMultiply;
}

//SpellProtection:
finalDamage = finalDamage - (finalDamage * SpellProtection(0-1));

//armorProtection
finalDamage = finalDamage - armor;

attribute->Health -= finalDamage;
}
*/