// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "GameplayTagContainer.h"
#include "../Structs/RPGSystemSructs.h"
#include "RPGEffectBase.h"
#include "RPGEffectUtility.generated.h"

UCLASS(Blueprintable, BlueprintType, MinimalAPI)
class URPGEffectUtility : public URPGEffectBase
{
	GENERATED_UCLASS_BODY()

public:
	virtual void PreInitialize() OVERRIDE;
	virtual bool Initialize() OVERRIDE;

	virtual void Deinitialize() OVERRIDE;
};
