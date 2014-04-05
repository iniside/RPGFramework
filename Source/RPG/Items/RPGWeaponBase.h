// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "RPGWeaponBase.generated.h"

/*
	Weapon Types.
	If there is new weapon. Add it here.
*/
UENUM()
enum EWeaponType
{
	Staff,
	MachineGun,
	OneHandedSword,
	HandGun,
	HeavyMachineGun,
	TwoHandedSword,
	Dagger,
	Bow,
	Crossbow //list is stub,but will do for testing.
};

/**
 * It should derive from ARPGItem, as it is base class for equipable items!
 */
UCLASS(Blueprintable)
class ARPGWeaponBase : public AActor
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=WeaponMesh)
	TSubobjectPtr<USkeletalMeshComponent> WeaponMesh;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=WeaponMesh)
	//USkeletalMesh* WeaponMesh;
	TSubobjectPtr<UArrowComponent> ArrowComp;

	UPROPERTY(EditAnywhere, Category=WeaponBaseProperties)
	TEnumAsByte<EWeaponType> WeaponType;
};
