// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "RPG.h"
#include "../Components/RPGAttributeBaseComponent.h"
#include "DamagefunctionStatics.h"


UDamagefunctionStatics::UDamagefunctionStatics(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	
}


float UDamagefunctionStatics::SpellDamage(AActor* Target, AActor* Owner, float InputValue)
{
	TWeakObjectPtr<URPGAttributeBaseComponent> TargetAttr = Target->FindComponentByClass<URPGAttributeBaseComponent>();
	TWeakObjectPtr<URPGAttributeBaseComponent> OwnerAttr = Owner->FindComponentByClass<URPGAttributeBaseComponent>();
	
	if (!OwnerAttr.IsValid() || !TargetAttr.IsValid())
		return 0;


	/*
		This is really simplified. I haven't really thought about any forumlas yet
		but you got the idea. Just remember the order in which you perform calculations is crucial.
	*/

	InputValue = InputValue + (OwnerAttr->GetIntelligence() / 2);

	//now we can do some defense for example:

	InputValue = InputValue - (TargetAttr->GetIntelligence() / 4);

	return InputValue;
}

//this is just for  checking chaining in blueprint. Have fun!
float UDamagefunctionStatics::FireDamage(AActor* Target, AActor* Owner, float InputValue)
{
	TWeakObjectPtr<URPGAttributeBaseComponent> TargetAttr = Target->FindComponentByClass<URPGAttributeBaseComponent>();
	TWeakObjectPtr<URPGAttributeBaseComponent> OwnerAttr = Owner->FindComponentByClass<URPGAttributeBaseComponent>();

	if (!OwnerAttr.IsValid() || !TargetAttr.IsValid())
		return 0;
	InputValue = InputValue + (OwnerAttr->GetIntelligence() / 5);

	return InputValue;
}