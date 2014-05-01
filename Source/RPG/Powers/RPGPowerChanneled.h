// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
//#include "../RPGCharacter.h"
#include "RPGPowerBase.h"
#include "RPGPowerChanneled.generated.h"

UCLASS(BlueprintType, Blueprintable)
class URPGPowerChanneled : public URPGPowerBase
{
	GENERATED_UCLASS_BODY()
	
	virtual void Tick(float DeltaTime)  OVERRIDE;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=BaseProperties)
	float MaxChannelTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=BaseProperties)
	float ChannelInterval;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=BaseProperties)
	float EnergyCostPerTick;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=BaseProperties)
	float HealthCostPerTick;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=BaseProperties)
	float EnduranceCostPerTick;

	virtual void OnCastStart() OVERRIDE;
	virtual void OnCastStop() OVERRIDE;
protected:
	void StartChannel();
	void StopChannel();
	void CastPower();
private:
	float channelIntervalElapsed;
	bool isChanneled;
};
