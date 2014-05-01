// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
//#include "../RPGCharacter.h"
#include "RPGPowerBase.h"
#include "RPGPowerInstant.generated.h"


UCLASS(BlueprintType, Blueprintable)
class URPGPowerInstant : public URPGPowerBase
{
	GENERATED_UCLASS_BODY()

	virtual void Tick(float DeltaTime)  OVERRIDE;
	virtual void OnCastStart() OVERRIDE;

private:
	void CastPower();
};
