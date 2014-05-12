#pragma once
#include "RPGDamageType.generated.h"
/**
Why bother with implementing damage types ? Game can perfectly work without them, here are my reason s why I decided to use them:
1. There are many system already implemented that handle damage dealing directly in engine. Various events, that take place when 
actor is hit and take damage. It's possible to recreate these systems, but why invent whell from scratch again ?
2. Damage doesn't mean, it hurt actor health. Damage mean everything that takes place in actor TakeDamage().
TakeDamage() on it's own is pretty limited, as it can take only single float value (for damage), and by extension can damage only 
single character property
3. To mitigate this limitation I decided to use damage type. Each damage type implement it's own damage dealing method. 
This method MUST be overrided in all class derived from URPGDamageType, as default implementation is not provided (and should not 
be provided!)
4. The amount of final damage that actor (character) will take is determined in two stages.
4a. In blueprint, there is set per-power forumla, that will calculate ability/power damage. For example magic*5 + int.
4b. In damage type we will calculate rest of affecting properties. Like resistance value, bonus damage 
by target of appiled by caster), armor reduction etc.
4c. It is entirely possible to make part b in blueprint. But it will introduce another complexity that will need to be taken care of,
and more oppurtinties to make mistakes. You can forgot to add nessesary mechanics to ability. Although it's viable
to make specific cases when blueprint will handle entire logic.
4d. It's also worth noting that system should be able to response for real-time changes. For example presitent effects can modify 
outgoing damage or damage taken, by flat values (+20, -10) or by precentage.
All of these effects are stored within data structures (structs) in Common/RPGCharacterStructs.h (might change!).
All character should have these included. These structs provide all possible modifications that can be run.
Damage calculations should be constructed in the way, that they can take care of 0 values in these structs.

Please note it is unrealistic that damage type will predict what data will be. It will work only on data that was passed on it
during calling DealDamage()!. To specific data, that is hold in memorry for affected actor in specific struct. There will be small delays in calling code. It shouldn't be big issue,
but if 0.1-1ms delay is issue, you should consider creating some kind queue and buffer mechanism, that will make sure
that right data will be passed to function.

To make example:
Target character have temporary debuff that will increase incoming magical damage by 30%, it lasts for 15s. How do we handle it ?
 And have 30% resitance (reduction) to magic damage.
 For example Magic Missle do 10 base damage.
 Here is example formula
 FinalDamage = (10 * 0.3) + 10 = 13
 FinalDamage = 13 - (13 *0.3) = 9.1
 So character will take 9.1 dmage.
 It's important to be consistent and/or implement different cases. For example one might first apply damage reduction from target
 and then apply bonus damage to get final result.
 Although most player will expect shown above behavior as more logical.

 DamageTypes are also used to apply damage to attributes. Like magic, intelligence, strenght constitiun. Can be used
 reduce character movment speed etc. It is all done with combination of data structs and implementing DealDamage function.

*/
UCLASS(MinimalAPI, const, Blueprintable, BlueprintType)
class URPGDamageType : public UDamageType
{
	GENERATED_UCLASS_BODY()
	
	UFUNCTION()
	virtual float DealDamage(class ARPGCharacter* causer, float damageAmount);

	FORCEINLINE void SetDamageTarget(URPGAttributeBaseComponent* target) { DamageTarget = target; };
	FORCEINLINE void SetDamageCauser(URPGAttributeBaseComponent* causer) { DamageCauser = causer; };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, EditInline, Category = "Lol")
		float BaseDamage;
protected:
	class URPGAttributeBaseComponent* DamageTarget;
	class URPGAttributeBaseComponent* DamageCauser;
};