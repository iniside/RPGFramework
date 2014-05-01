// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Object.h"
#include "../../Common/RPGSharedEnums.h"
#include "RPGConditionBase.generated.h"

//class ARPGCharacter;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class URPGConditionBase : public UObject, public FTickableGameObject
{
	GENERATED_UCLASS_BODY()

	virtual void Tick(float DeltaTime)  OVERRIDE;
	virtual bool IsTickable() const OVERRIDE;
	virtual TStatId GetStatId() const OVERRIDE;

	void Initialize();
	void SetTarget(class ARPGCharacter* target);
	void SetAppiledBy(class ARPGCharacter* appiledBy);

	virtual void ApplyCondition();

	/*
	* If TickInterval or MaxTickCount is 0, set this value
	* usefull for conditions that do not have any ticks,
	* just presitent effect over time.
	*/
	UPROPERTY(EditAnywhere, Category=BaseProperties)
	float Duration;
	UPROPERTY(EditAnywhere, Category = BaseProperties)
	float TickInterval;
	UPROPERTY(EditAnywhere, Category = BaseProperties)
	int32 MaxTickCount;

	EConditionType ConditionType;

protected:
	UPROPERTY(BlueprintReadOnly, Category="NativeVars|Character")
	class ARPGCharacter* AffectedTarget;
	UPROPERTY(BlueprintReadOnly, Category = "NativeVars|Character")
	class ARPGCharacter* AppiledBy;


	float currentDuration;
	float currentTickInterval;
	int32 currentTick;
	bool useDuration;
protected:
	virtual class UWorld* GetWorld() const OVERRIDE;

};
