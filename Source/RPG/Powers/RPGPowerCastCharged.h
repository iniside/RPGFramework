// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
//#include "../RPGCharacter.h"
#include "RPGPowerBase.h"
#include "RPGPowerCastCharged.generated.h"


UCLASS(BlueprintType, Blueprintable)
class URPGPowerCastCharged : public URPGPowerBase
{
	GENERATED_UCLASS_BODY()

	void Tick(float DeltaTime) OVERRIDE;

	UPROPERTY(EditAnywhere, Category = BaseProperties)
	float CastTime;
	UPROPERTY(EditAnywhere, Category = BaseProperties)
	float MaxTimeOverchargeTime;
	UPROPERTY(EditAnywhere, Category = BaseProperties)
	bool UseOvercharge;

	void OnCastStart() OVERRIDE;
	void OnCastStop() OVERRIDE;

protected:
	void CastPower();
private:
	bool isCharged;
	float currentChargeTime;
	float GetCurrentChargeTime();
};
