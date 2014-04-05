// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
//#include "RPGPlayerController.h"
#include "RPGProjectile.generated.h"

/**
 * Base projectile class.
 * as with everything in this project implement only helper functions,
 * and leave game logic/mechanics to blueprints.
 * Projectiles are physical actors, that have travel speed, can collide with arbitary objects on path,
 * can bounce, track objects, and hell know what else...
 * Projectiles are spawned by weapons and powers.
 */
UCLASS()
class ARPGProjectile : public AActor
{
	GENERATED_UCLASS_BODY()
	/** initial setup */
	virtual void PostInitializeComponents() OVERRIDE;

	/** setup velocity */
	void InitVelocity(FVector& ShootDirection);

	/** handle hit */
	//UFUNCTION()
	void OnImpact(FHitResult const& HitResult);
	
	/** Bounces */
	void OnBounce(FHitResult const& HitResult);

	UFUNCTION(BlueprintImplementableEvent, Category=ProjectileEvents)
	void OnProjectileImpact();
	UFUNCTION(BlueprintImplementableEvent, Category=ProjectileEvents)
	void OnProjectileDestroyed();
	UFUNCTION(BlueprintImplementableEvent, Category=ProjectileEvents)
	void OnProjectileSpawned();

public:
	
	/** Projectile Lifetime in Seconds */
	UPROPERTY(EditDefaultsOnly, Category=ProjectileNativeProperties)
	float ProjectileLifeTime;
	
	/** Collision Radius */
	UPROPERTY(EditDefaultsOnly, Category=ProjectileNativeProperties)
	float CollisionSphereRadius;
	
	/** Acceleration Multiplier EVERY TICK, can increase velocity up to VictoryProjMaxSpeed*/
	UPROPERTY(EditDefaultsOnly, Category=ProjectileNativeProperties)
	float ProjectileAcceleration;
	
	void InitializeProjectile();

protected:

	//Tick
	virtual void Tick(float DeltaSeconds) OVERRIDE;
	
	/** movement component */
	UPROPERTY(VisibleDefaultsOnly, Category=ProjMovement)
	TSubobjectPtr<UProjectileMovementComponent> MovementComp;

	/** collisions */
	UPROPERTY(VisibleDefaultsOnly, Category=ProjCollision)
	TSubobjectPtr<USphereComponent> CollisionComp;
	
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	TSubobjectPtr<UParticleSystemComponent> ParticleComp;

	/** effects for explosion */
	UPROPERTY(EditDefaultsOnly, Category=Effects)
	UParticleSystem* ExplosionTemplate;

	/*
		Particle Spawned when projectile is moving.
		We assume particle, since it is most universal actor.
	*/
	UPROPERTY(EditDefaultsOnly, Category=Effects)
	UParticleSystem* FXProjectileTravel;

	/** Owning controller */
	
	/*
		We need access to shooter character properties,
		so we can setup thing like damage if we so desire.
		Besides it will give us direct access to information, who shot projectile
		if we need it on impact, or on actor who will be hit.
	*/
	UPROPERTY(BlueprintReadWrite, Category=NativeHelpers)
	class ARPGCharacter* ProjectileShooter;

	class ARPGPlayerController* MyController;

	/** did it explode? */
	UPROPERTY(Transient)
	bool bExploded;

	/** [client] explosion happened */
	UFUNCTION()
	void OnRep_Exploded();

	/** trigger explosion */
	void Explode(const FHitResult& Impact);

	/** shutdown projectile and prepare for destruction */
	void DisableAndDestroy();

	/** update velocity on client */
	virtual void PostNetReceiveVelocity(const FVector& NewVelocity) OVERRIDE;
	
};
