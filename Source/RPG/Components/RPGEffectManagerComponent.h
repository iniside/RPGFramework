// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "RPG.h"
#include "../Effects/RPGEffectBase.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=EffectList)
	TArray<class URPGEffectBase*> EffectsList;

	void AddEffect(URPGEffectBase* newEffect);

	void DestroyEffect(URPGEffectBase* EffectToDestroy);

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) OVERRIDE;
	
	void RemoveEffect(URPGEffectBase* effectToRemove);
	//void RemoveEffectByType(TEnumAsByte<EEffectType> effectType);
private:
	URPGEffectBase* NewEffect;
	float tickTime;
	float totalTickTime;

	//inline static bool EffectTypePredicate (const URPGEffectBase& ip1, TEnumAsByte<EEffectType> ip2)
 //   {
	//	return (ip1.EffectType == ip2);
 //   }
};
