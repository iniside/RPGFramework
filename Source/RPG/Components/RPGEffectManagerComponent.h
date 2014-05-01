// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "RPG.h"
#include "../Common/RPGCharacterStructs.h"
#include "RPGEffectManagerComponent.generated.h"

/**
 * TODO:
 * 1. Manage effect duration
 * 2. Manage effect Stacking.
 * 3. Manage Effect Refreshing.
 * 4. 
 */
/**
* DONE:
* 1. Effect manager now manage remocing and adding effects!
* 2. Effect manager now, manages per frame tick events, for effects!
*/
UCLASS(meta=(BlueprintSpawnableComponent))
class URPGEffectManagerComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category=EffectList)
	TArray<class URPGEffectBase*> EffectsList;

	UPROPERTY(BlueprintReadWrite, Category=ConditionsList)
	TArray<class URPGConditionBleed*> ConditionBleeds;

	void AddBleed(class URPGConditionBleed* condition);
	void RemoveBleed(class URPGConditionBleed* condition);
	void AddEffect(class URPGEffectBase* newEffect);

	void DestroyEffect(class URPGEffectBase* EffectToDestroy);

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) OVERRIDE;
	
	void RemoveEffect(class URPGEffectBase* effectToRemove);

	void AddCondition(class URPGConditionBase* condition);

	void RemoveCondition(class URPGConditionBase* condition);

	void DestroyCondition(class URPGConditionBase* condition);
	//void RemoveEffectByType(TEnumAsByte<EEffectType> effectType);

	/*
	 * Effects On Character
	 */
	UPROPERTY(BlueprintReadOnly, Category = EffectsOnCharacter)
	uint8 Hexes;
	
	UPROPERTY(BlueprintReadOnly, Category = EffectsOnCharacter)
	uint8 Enchantments;
	
	UPROPERTY(BlueprintReadWrite, Category = EffectsOnCharacter)
	uint8 Curses;
	/*
	* This overall number of conditions, without distinguishing by type.
	*/
	UPROPERTY(BlueprintReadOnly, Category = EffectsOnCharacter)
	uint8 Conditions;

	UPROPERTY(BlueprintReadOnly, Category = EffectsOnCharacter)
	uint8 Boons;

private:
	URPGEffectBase* NewEffect;
	float tickTime;
	float totalTickTime;

	//inline static bool EffectTypePredicate (const URPGEffectBase& ip1, TEnumAsByte<EEffectType> ip2)
 //   {
	//	return (ip1.EffectType == ip2);
 //   }
};
