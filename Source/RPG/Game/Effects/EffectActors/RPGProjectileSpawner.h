// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "../RPGEffectActor.h"
#include "RPGProjectileSpawner.generated.h"

/**
 * Base class for presitent actor effect,
 * Actor effect is usually spawned by power (or other effect)
 * Actor effect can presist set amount of time or idefinetly (until destruction by other actors)
 * Actor effect might have health.
 * actor effect is always spawned on level with static mesh or with other indicator that 
 * it is in place.
 * Effect Actor might have component, that will add combo field.
 * combo field is special componenent with it's own collision channel (overlap).
 * when two combo componenets overlap, they remove both effects and spawn new one
 * which is defined by designer
 * There are special cases that do not remove combo field, but instead transfer it effect further
 * (like projectile, whirlwind or wind).
 */
/*
 * key difference between Object Effect and Actor Effect is
 * that actor exisits completly on it's own, and doesn't need to be attached
 * to other actor.
 * it can interact with any other actor, using all available unreal systems.
*/

UCLASS()
class ARPGProjectileSpawner : public ARPGEffectActor
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, Category = BasicProperties)
		float Radius;

	UPROPERTY(EditAnywhere, Category = Projectile)
		TSubclassOf<class ARPGProjectile> Projectile;

	//how much time passes between projectile spawns.
	UPROPERTY(EditAnywhere, Category = BasicProperties)
		float TimeInterval;

	void Tick(float DeltaTime) OVERRIDE;

	UFUNCTION(BlueprintCallable, Category = ProjectileSpanwer)
		void SpawnProjectile();

	void OnConstruction(const FTransform& Transform) OVERRIDE;

	UFUNCTION(BlueprintImplementableEvent, Category = Events)
		void OnSpawned();

	FVector SpawnLocation;

	void SetSpawnLocation(FVector location);
};
