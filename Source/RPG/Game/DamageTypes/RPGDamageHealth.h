#pragma once
#include "RPGDamageType.h"
#include "RPGDamageHealth.generated.h"

UCLASS(MinimalAPI, const, Blueprintable, BlueprintType)
class URPGDamageHealth : public URPGDamageType
{
	GENERATED_UCLASS_BODY()

	
	virtual float DealDamage(class ARPGCharacter* causer, float damageAmount) OVERRIDE;
};