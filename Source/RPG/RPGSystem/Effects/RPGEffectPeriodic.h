// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Object.h"
#include "Tickable.h"
#include "GameplayTagContainer.h"
#include "../Structs/RPGSystemSructs.h"
#include "RPGEffectBase.h"
#include "RPGEffectCombat.h"
#include "RPGEffectPeriodic.generated.h"

UCLASS(Blueprintable, BlueprintType, MinimalAPI)
class URPGEffectPeriodic : public URPGEffectCombat, public FTickableGameObject
{
	GENERATED_UCLASS_BODY()

	virtual void Tick(float DeltaTime)  OVERRIDE;
	virtual bool IsTickable() const OVERRIDE;
	virtual TStatId GetStatId() const OVERRIDE;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "RPG|Effect")
		float CurrentDuration;

	UPROPERTY(EditAnywhere, Category = "Periodic Effect")
		float PeriodLenght;

	UPROPERTY(EditAnywhere, Category = "Periodic Effect")
		float PeriodsCount;

public:
	FORCEINLINE void SetPeriodLenght(float value) { PeriodLenght = value; };
	FORCEINLINE void SetPeriodCount(float value) { PeriodsCount = value; };

	UPROPERTY(EditAnywhere, Category = "Base Properties")
		bool StackDuration;

	UPROPERTY()
		float TotalDuration;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Effect")
		FOnEffectPeriod OnEffectPeriodHit;

private:
	float currentPeriodTime;
public:
	virtual void PreInitialize() OVERRIDE;
	virtual bool Initialize() OVERRIDE;

	virtual void Deinitialize() OVERRIDE;

	/*
		Possibly move these up in hierarchy ?
	*/
	/*
		Cache Instant effects to array. Do it before using, 
	*/
	UFUNCTION(BlueprintCallable, Category = "RPG|Effect")
		void RunEffects();
protected:
	void SelfRemoveEffect();
};
