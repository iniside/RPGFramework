#pragma once
//#include "RPG.h"
#include "RPGCharacterStructs.generated.h"


USTRUCT(BlueprintType)
struct FCharacterStructs
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float AwsomeProperty;
};

USTRUCT(BlueprintType)
struct FCharacterCombat
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, Category=EffectsOnCharacter)
	float AttackSpeed;

	UPROPERTY(BlueprintReadWrite, Category=EffectsOnCharacter)
	float CastingSpeed;

	UPROPERTY(BlueprintReadWrite, Category=EffectsOnCharacter)
	float Weight;
};

/*
This struct contains information on things that are affecting character. Most of them are types of Effects dervied from URPGEffectBase.
If effect of said type is appiled to chacater it is number in this struct is increased. By default all should be 0.
After said effect expires and stop affecting character, number on struct prperty is subtracted.

Data from this struct is mainly used by other powers/effect to determine their mechanics.
*/
USTRUCT(BlueprintType)
struct FEffectsOnCharacter
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, Category=EffectsOnCharacter)
	uint8 Hexes;

	UPROPERTY(BlueprintReadWrite, Category=EffectsOnCharacter)
	uint8 Enchantments;

	UPROPERTY(BlueprintReadWrite, Category=EffectsOnCharacter)
	uint8 Curses;

	UPROPERTY(BlueprintReadWrite, Category=EffectsOnCharacter)
	uint8 Conditions;

	UPROPERTY(BlueprintReadWrite, Category=EffectsOnCharacter)
	uint8 Boons;
};
/*
Character conditions.
Appiling should be handled entirely trough code.
It will be easier this way, and simplify blueprints, as we want 
condition to be special case. (they can stack in effect intensity or duration or both).
Creating blueprint that can handle this behaviiour would be way to convoluted.
Besides. Each new condition type must be represented in this struct anyway.
It's easier to keep shit in one place, in this case.
*/
USTRUCT(BlueprintType)
struct FCharacterCondition
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadWrite, Category=CharacterStatus)
	bool IsBurning;
	UPROPERTY(BlueprintReadWrite, Category=CharacterStatus)
	int32 BurningStacks;

	UPROPERTY(BlueprintReadWrite, Category=CharacterStatus)
	bool IsSlowed;

	UPROPERTY(BlueprintReadWrite, Category=CharacterStatus)
	bool IsFreezed;

	UPROPERTY(BlueprintReadWrite, Category=CharacterStatus)
	bool IsFeared;

	UPROPERTY(BlueprintReadWrite, Category=CharacterStatus)
	bool IsBleeding;
	UPROPERTY(BlueprintReadWrite, Category=CharacterStatus)
	int32 BleedingStacks;
};

/*
This struct setup data for character feats. 
It's easier to just set simple values in struct, rather than trying to delegate 
special values from Feat object to another object that might need it at the moment.
Although it can get messy, because of amount of Properties, 
and if said data affects something like power, weapon, etc. 
you must remember to take values from this struct into consideration.

It could be easy to lost track of it.

That's why common function should encaspulate struct access and do it behind the scene, and all logic should be delegated to blueprint ;).
There might be better solution though.


*/
USTRUCT(BlueprintType)
struct FCharacterFeats
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, Category=EffectsOnCharacter)
	bool CanCastSpell;

	/*
		Can character be stunned.
	*/
	UPROPERTY(BlueprintReadWrite, Category=EffectsOnCharacter)
	bool CanBeStunned;
};