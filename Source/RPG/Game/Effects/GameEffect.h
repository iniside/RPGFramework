// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Object.h"
#include "Tickable.h"
#include "../../RPGSystem/Effects/RPGEffectBase.h"
#include "../Shared/RPGEnums.h"
#include "GameEffect.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UGameEffect : public URPGEffectBase
{
	GENERATED_UCLASS_BODY()

	virtual void PreInitialize() OVERRIDE;
	virtual bool Initialize() OVERRIDE;

	UPROPERTY(EditAnywhere, Category = "Effect Type")
		TEnumAsByte<EEffectType> EffectType;

protected:
	
	UPROPERTY(BlueprintReadOnly, Category = "Effect|Target|Attributes")
		float TargetHealth;
	UPROPERTY(BlueprintReadOnly, Category = "Effect|Target|Attributes")
		float TargetEnergy;
	UPROPERTY(BlueprintReadOnly, Category = "Effect|Target|Attributes")
		float TargetStamina;

private:
	void SetTargetAttributeUpdate();
};
