// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Object.h"
#include "RPGConditionBase.h"
#include "RPGConditionBleed.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class URPGConditionBleed : public URPGConditionBase
{
	GENERATED_UCLASS_BODY()

	virtual void Tick(float DeltaTime)  OVERRIDE;

	virtual void ApplyCondition() OVERRIDE;
protected:
	virtual class UWorld* GetWorld() const OVERRIDE;
};
