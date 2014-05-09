// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "RPG.h"
#include "RPGAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCallMeEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttributeChanged);
//DECLARE_DELEGATE_RetVal(URPGPowerBase, FOnNewPowerAdded);
/**
 * Idea for this, come from Epic source (you can look it Runtime/Skillsystem).
 * It is supposed to completly decouple attributes from actor, and by extensions
 * decouple things like Effects, Powers, Deadth from pawn as well.
 * all those things should just check if actor have this component.
 * if it doesn't they simply ignore actor, as they can't affect it.
 * Things to consider:
 * Damage handling - I still wan't to make use of Unreal systems. Like TakeDamage, and DamgeType
 * I can't see how I could use TakeDamage, without coupling again component to Pawn
 * so component will probably need to have it's own Damage handling logic, along with events
 * and blueprint functions.
 * Damage types can be probably still levelraged, to calculate damage
 * but that would mean that all damage types with RPG* would be coupled to this component
 * and attributes it contain. Not very flexible system, but greatly simplify design.
 * and attributes could be simply modified by modifying this component.
 *
 * Attributes - should they be hard coded to this component
 * or they should be declared as separate class(es)
 * and component would only hold array of them.
 *
 * Should effect compoentn be merged with Attributes ?
 * those component are pretty interconnected anyway...
 * Effects in the essence, modify attributes
 */
UENUM()
enum EAttributeChange
{
	Add,
	Subtract,
	Multiply,
	Divide,
	Override
};

UCLASS(meta = (BlueprintSpawnableComponent), hidecategories = (Object, LOD, Lighting, Transform, Sockets, TextureStreaming))
class URPGAttributeComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) OVERRIDE;
	virtual void InitializeComponent() OVERRIDE;
	virtual void OnRegister() OVERRIDE;
public:
	void TakeDamage(float Damage, TSubclassOf<class URPGDamageType> DamageType);

	UPROPERTY(BlueprintAssignable, Category=AttributeEvents)
	FOnCallMeEvent OnCallMeEvent;
	UPROPERTY(BlueprintAssignable, Category = AttributeEvents)
	FOnAttributeChanged OnAttributeChanged;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = AttributeEvents)
	void OnDamageAppiled();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = AttributeEvents)
	void OnActorDeath();

	//Effect Managment
public:
	UPROPERTY(BlueprintReadWrite, Category = EffectList)
		TArray<class URPGEffectBase*> EffectsList;

	void AddEffect(class URPGEffectBase* newEffect);
	void RemoveEffect(class URPGEffectBase* effectToRemove);

protected:
	void DestroyEffect(class URPGEffectBase* EffectToDestroy);
};

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