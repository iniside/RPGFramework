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

	virtual void InputPressed() OVERRIDE;
	virtual void InputReleased() OVERRIDE;
protected:
	virtual void Initialize(APawn* owner, AController* instigator) OVERRIDE;

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

	UFUNCTION(BlueprintCallable, Category = "Ability|Data")
		void GetTableRow(UDataTable* Data, FName RowID, FAttributeData& OutData);

protected:
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
	TWeakObjectPtr<class URPGAttributeBaseComponent> OwnerAttributes;

	//UPROPERTY(BlueprintReadOnly, Category = "Ability")
	//TWeakObjectPtr<class URPGAttributeBaseComponent> TargetAttributes;

	TWeakObjectPtr<class URPGEquipmentManagerComponent> EquipComp;
	class ARPGWeaponBase* MainHand;
	class ARPGWeaponBase* OffHand;
	/*
	Calculates final cost of ability.
	If the final cost if bigger than remainin resource it return false;
	If return true, it also subtract cost from character energy pool.
	*/
	bool CalculateFinalCost(float energyCost, float healthCost, float staminaCost);
	UFUNCTION(BlueprintCallable, Category = "Ability|Helpers")
	FVector GetCameraAim() const;

private:
	FHitResult RangedPowerTrace(const FVector& StartTrace, const FVector& EndTrace) const;
	FVector GetCameraDamageStarLocation(const FVector& AimDir) const;

	USkeletalMeshComponent* GetWeaponMesh() const;
	FVector GetCastingLocation(FName SocketName) const;
	FVector GetMuzzleDirection();
	void SetTargetAttributes(AActor* targetActor);
};
