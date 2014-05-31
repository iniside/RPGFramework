// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "GameplayTagContainer.h"
#include "../Structs/RPGSystemSructs.h"
#include "RPGEffectBase.h"
#include "RPGEffectUtility.h"
#include "RPGEffectAreaSpread.generated.h"

UCLASS(Blueprintable, BlueprintType, MinimalAPI)
class URPGEffectAreaSpread : public URPGEffectUtility
{
	GENERATED_UCLASS_BODY()

public:
	virtual void PreInitialize() OVERRIDE;
	virtual bool Initialize() OVERRIDE;

	virtual void Deinitialize() OVERRIDE;

	TWeakObjectPtr<class URPGEffectCombat> EffectToSpread;

	TArray<TWeakObjectPtr<class URPGEffectCombat>> EffectsToSpred;

	FVector HitLocation;

	float Radius;

	float MaxTargets;

	//UFUNCTION(BlueprintCallable, Category = "RPG|Effect")
	//	TArray<AActor*> SpreadEffect();

	//UFUNCTION(blueprintCallable, Category = "RPG|Effect")
	//	TArray<AActor*> SpreadEffectThroughActors(TArray<AActor*> TargetActors);

	virtual class UWorld* GetWorld() const OVERRIDE;
};
