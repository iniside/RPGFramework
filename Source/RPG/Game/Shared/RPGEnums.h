#pragma once
//#include "RPG.h"
#include "RPGEnums.generated.h"

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
enum EWeaponType
{
	Staff,
	MachineGun,
	OneHandedSword,
	HandGun,
	HeavyMachineGun,
	TwoHandedSword,
	Dagger,
	Bow,
	Crossbow
};