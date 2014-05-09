// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Object.h"
#include "../RPGSystem/Abilities/RPGAbilityBase.h"
#include "../Shared/RPGEnums.h"
#include "GameAbility.generated.h"


UCLASS(BlueprintType, Blueprintable)
class UGameAbility : public URPGAbilityBase
{
	GENERATED_UCLASS_BODY()

	virtual void Tick(float DeltaTime)  OVERRIDE;

	virtual void StartAbility() OVERRIDE;
	virtual void StopAbility() OVERRIDE;
protected:
	virtual bool Initialize() OVERRIDE;

public:
	UPROPERTY(EditAnywhere, Category = "Ability Requierments")
		TEnumAsByte<EWeaponType> RequiredWeapon;

	UPROPERTY(EditAnywhere, Category = "Ability Requierments")
		bool CostPerTick;

public:

	UFUNCTION(BlueprintCallable, Category = "Ability|Helpers")
	FHitResult GetHitResult(float Range, FName StartSocke);

	UFUNCTION(BlueprintCallable, Category = "Ability|Helpers")
	AActor* GetActorHit(float Range, FName StartSocke);

	UFUNCTION(BlueprintCallable, Category = "Ability|Helpers")
		void SpawnTrailEffect(UParticleSystem* trailFX, float trailSpeed, FName trailSpeedParam, FHitResult target, FName SocketName);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Cost")
		float BaseHealthCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Cost")
		float BaseEnergyCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Cost")
		float BaseStaminaCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Cost")
		float PeriodicBaseHealthCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Cost")
		float PeriodicBaseEnergyCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability|Cost")
		float PeriodicBaseStaminaCost;
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Ability")
	class URPGAttributeBaseComponent* OwnerAttributes;

	UPROPERTY(BlueprintReadOnly, Category = "Ability")
	class URPGAttributeBaseComponent* TargetAttributes;

	class URPGEquipmentManagerComponent* EquipComp;
	class ARPGWeaponBase* MainHand;
	class ARPGWeaponBase* OffHand;
	/*
	Calculates final cost of ability.
	If the final cost if bigger than remainin resource it return false;
	If return true, it also subtract cost from character energy pool.
	*/
	bool CalculateFinalCost(float energyCost, float healthCost, float staminaCost);
private:
	FHitResult RangedPowerTrace(const FVector& StartTrace, const FVector& EndTrace) const;
	FVector GetCameraDamageStarLocation(const FVector& AimDir) const;
	FVector GetCameraAim() const;
	USkeletalMeshComponent* GetWeaponMesh();
	FVector GetCastingLocation(FName SocketName);
	FVector GetMuzzleDirection();
	void SetTargetAttributes(AActor* targetActor);
};
