#pragma once
//#include "RPG.h"
#include "RPGSharedEnums.generated.h"


USTRUCT(BlueprintType)
struct FEmptyStructForLulz
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float AwsomeProperty;
};

/*
I'm honestly not sure if enum is right choice here.
It might better to create subclass for each of the effect types as it would allow bigger flexibility,
and then just compare types of class to check what to do with them.
But on other hand comparing types on runtime is very bad practice and should be avoided.
For example Effect might be Hex amd Damage Over Time at the same time, but we must choose only from one.
On other hand it might be not that bad, since designer who will create effect in blueprint will be
forced to be very specfici about effect he create, and stick to guideline about what each type of
effect should do.
Besides, effects can be nested (one effect can apply another effect, etc).

In any case this enum is directly tied to Common/RPGCharacterStructs.h, and struct FEffectsOnCharacter.
Properties of this struct should match enum values! - UPDATE THIS COMMENT IF IT CHANGE!
*/
UENUM()
enum EEffectType
{
	Effect_Hex,
	Effect_Enchantment,
	Effect_Curse,
	Effect_Condition,
	Effect_Boon,
	Effect_Disease
};
UENUM()
enum EPowerType
{
	Power_Spell,
	Power_Mele,
	Power_Ranged,
	Power_RangedPhysical
};
UENUM()
enum EConditionType
{
	Cond_Bleed,
	Cond_Poison,
	Cond_Burning,
	Cond_Wounded,
	Cond_Blind,
	Cond_Crippled,
	Cond_Weakness,
	Cond_Dazed,
	Cond_Fear
};