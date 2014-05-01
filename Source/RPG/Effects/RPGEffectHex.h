// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Object.h"
#include "RPGEffectBase.h"
#include "RPGEffectHex.generated.h"

UCLASS(Blueprintable, BlueprintType)
class URPGEffectHex : public URPGEffectBase
{
	GENERATED_UCLASS_BODY()

	virtual void AddEffect() OVERRIDE;

protected:
	virtual void SelfRemoveEffect() OVERRIDE;
};
