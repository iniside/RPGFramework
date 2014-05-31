// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "../RPGSystem/RPGAttributeBase.h"
#include "DamagefunctionStatics.generated.h"

/*
	Purpose Of this class is very simple.
	Add all static damage, healing, whatever calculations here in form of static functions.
	So you can have base functions for generic calculation of things like Spell Damage, Fire Damage, 
	Physical Damage bla bla, you got the idea.

	The impportant part is that functions can be chained togather. So you can create something like 
	Spell Fire Damage. So you first call function that calculate Spell Damage, and then you plug result
	of it to Fire Damage function, and then.. You got the point.

	This is extremly game specific. As each game can have it's own damage calulation formulas.
	Never the less coding these functions is very easy, and literally everybody should be able to handle it.
	Just open this file, copy existing function, change body of function to make calculations you need,
	return result. 

	You can also scrap this, and just make static function library purely in blueprint. Up to you.
	It's mainly to present idea, of how this can be handled.
*/
/*
	AActor* Target - Target of Ability, effect, something, you want to use his attributes in calculations;
	AActor* Owner - Who owns/used/caused ability, effect or soemthing, you want to take his his attributes as well.

	For sake of simplicity we will use our customized AttributeComponent, so we don't have
	to get attribtues by reflection, we can simply get them by pointer.
	
*/


UCLASS()
class UDamagefunctionStatics : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintPure, Category = "Game|Damage Functions")
		static float SpellDamage(AActor* Target, AActor* Owner, float InputValue);

	UFUNCTION(BlueprintPure, Category = "Game|Damage Functions")
		static float FireDamage(AActor* Target, AActor* Owner, float InputValue);
};
