// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
//#include "../RPGCharacter.h"
#include "RPGPowerBase.h"
#include "RPGPowerCast.generated.h"


UCLASS(BlueprintType, Blueprintable)
class URPGPowerCast : public URPGPowerBase
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseProperties)
	float CastingTime;

	virtual void Tick(float DeltaTime)  OVERRIDE;
	virtual void OnCastStart() OVERRIDE;

private:
	void CastPower();
};
