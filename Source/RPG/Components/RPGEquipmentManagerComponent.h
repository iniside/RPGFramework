// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "RPG.h"
//#include "../RPGCharacter.h"
#include "../Items/RPGWeaponBase.h"
#include "../Items/RPGItem.h"
#include "RPGEquipmentManagerComponent.generated.h"

/**
 * 
 */
UCLASS()
class URPGEquipmentManagerComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY()
	class ARPGWeaponBase* MainWeapon;
	UPROPERTY()
	ARPGWeaponBase* OffHandWeapon;

	/*
		Secondary set for equiped weapons.
		Weapon in this set are, not technically equiped, so they can't be used.
		But they are stored here, for quick switching between various sets.
	*/
	UPROPERTY()
	ARPGWeaponBase* MainWeaponSet2;
	UPROPERTY()
	ARPGWeaponBase* OffHandWeaponSet2;

	UPROPERTY()
	ARPGItem* ChestSlot;

	UPROPERTY()
	ARPGItem* HeadSlot;

	UPROPERTY()
	ARPGItem* LegSlot;

	UPROPERTY()
	ARPGItem* FootSlot;

	UPROPERTY()
	ARPGItem* HandsSlot;

	UPROPERTY()
	ARPGItem* ShouldersSlot;

	UPROPERTY()
	TArray<ARPGItem*> EquipedItems;

	UFUNCTION(BlueprintCallable, Category=CharacterEquipment)
	void EquipItem(TSubclassOf<class ARPGItem> item, ARPGCharacter* character);

	UFUNCTION(BlueprintCallable, Category=CharacterEquipment)
	void UnEquipItem(ARPGItem* item, ARPGCharacter* character);

	UFUNCTION(BlueprintCallable, Category=CharacterEquipment)
	void EquipWeapon(TSubclassOf<class ARPGWeaponBase> weapon, ARPGCharacter* characterToAttach, FName SocketName);

private:
	void SetCharacterStats(ARPGItem* item, ARPGCharacter* character);
	void EquipChestItem(ARPGItem* item, ARPGCharacter* character);
	void EquipFootItem(ARPGItem* item, ARPGCharacter* character);
	inline static bool ConstPredicate (const ARPGItem& ip1, const ARPGItem& ip2) 
	{
		return (ip1.Attributes.Constitution > ip2.Attributes.Constitution);
	}
};

struct FConstitutionPredicate
{
    inline bool operator() (ARPGItem& ip1, ARPGItem& ip2)
    {
        return (ip1.Attributes.Constitution < ip2.Attributes.Constitution);
    }
};