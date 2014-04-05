// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
//#include "../Common/RPGCharacterAttributes.h"
#include "../RPGCharacter.h"
//#include "Items/RPGItemEffect.h"
#include "RPGItem.generated.h"

/**
 * 
 */

UENUM()
enum EItemType
{
	Item_Chest,
	Item_Head,
	Item_Legs,
	Item_Foot,
	Item_Hands,
	Item_Shoulders,
	Item_MainWeapon,
	Item_OffHandWeapon,
};


UCLASS()
class ARPGItem : public AActor
{
	GENERATED_UCLASS_BODY()

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ItemAttributes)
	FCharacterAttributes Attributes;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ItemAttributes)
	TEnumAsByte<EItemType> ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ItemAttributes)
	ARPGCharacter* ItemOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ItemAttributes)
	USkeletalMesh* ChestMesh;

	/**
	Arbitary number of effects that this item can apply to character.
	Please do not overuse it. Thanks ;)
	*/

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ItemEffects)
	//TArray<TSubclassOf<class URPGItemEffect> > DefaultEffectList;
	//UPROPERTY()
	//TArray<URPGItemEffect*> ItemEffects;

	/**
	Trigger special item effect, on ability activation/tick. We will call this event regardless and whether it do something or no, depends on item implmenentation.
	*/
	UFUNCTION(BlueprintNativeEvent, Category=ItemEvent)
	void OnItemProc();

	virtual void Tick(float DeltaSeconds) OVERRIDE;

	void InitializeItem();
	/*bool ConstituionPredicate(const ARPGItem& ip1, const ARPGItem& ip2);*/
private:
	void SpawnDefaultEffectList();
	//void AddEffect(URPGItemEffect* itemEffect);
};
