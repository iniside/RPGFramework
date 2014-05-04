// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "RPG.h"
//#include "../RPGCharacter.h"
#include "../Items/RPGWeaponBase.h"
#include "../Items/RPGItem.h"
#include "RPGEquipmentManagerComponent.generated.h"

UENUM()
enum EItemSlot 
{
	ChestSlot,
	HeadSlot,
	LegSlot,
	FootSlot,
	HandSlot,
	ShoulderSlot
};

/**
 * 
 */
UCLASS()
class URPGEquipmentManagerComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

public:

	virtual void OnComponentCreated() OVERRIDE;

	virtual void InitializeComponent() OVERRIDE;

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
	void EquipItem(TSubclassOf<class ARPGItem> item);

	UFUNCTION(BlueprintCallable, Category=CharacterEquipment)
	void UnEquipItem(ARPGItem* item);

	UFUNCTION(BlueprintCallable, Category=CharacterEquipment)
		void EquipWeapon(TSubclassOf<class ARPGWeaponBase> weapon, FName SocketName, TEnumAsByte<EItemSlot> itemSlot);

private:
	void SetCharacterStats(ARPGItem* item);
	void EquipChestItem(ARPGItem* item);
	void EquipFootItem(ARPGItem* item);
	
	uint8 EquipedItemsCount;
	AActor* EquipmentOwner;

	class URPGAttributeComponent* attributeComp;
	class USkeletalMeshComponent* ChestSlotComp;
	class USkeletalMeshComponent* FootSlotComp;
	class USkeletalMeshComponent* HeadSlotComp;
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