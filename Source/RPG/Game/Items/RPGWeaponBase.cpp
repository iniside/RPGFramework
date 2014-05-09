// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#include "RPG.h"
#include "RPGWeaponBase.h"


ARPGWeaponBase::ARPGWeaponBase(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	ArrowComp =  PCIP.CreateDefaultSubobject<UArrowComponent>(this, TEXT("ArrowComp"));
	WeaponMesh = PCIP.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("WeaponMesh"));
	WeaponMesh->AttachParent = ArrowComp;
	
}


