// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#include "RPG.h"
#include "RPGItem.h"


ARPGItem::ARPGItem(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;
}


void ARPGItem::OnItemProc_Implementation()
{
}

void ARPGItem::InitializeItem()
{
	SpawnDefaultEffectList();
}

void ARPGItem::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(ItemOwner) //check if item is owned by character
	{
		//if(ItemEffects.Num() > 0) //if there any effects assigned to item
		//{
		//	for(URPGItemEffect* effect : ItemEffects) //for effect
		//	{
		//		effect->OnAttributeChange(effect->CurrentCharacterAttributes); //we activate event that will send current character attributes to effect
		//		//it's not finished, and it should be REVISED
		//		effect->OnEffectTick();
		//	}
		//}
	}
}

void ARPGItem::SpawnDefaultEffectList()
{
	//if(DefaultEffectList.Num() > 0)
	//{
	//	for(int32 i = 0; i < DefaultEffectList.Num(); i++)
	//	{
	//		URPGItemEffect* itemEffect = ConstructObject<URPGItemEffect>(DefaultEffectList[i]);
	//		AddEffect(itemEffect);
	//	}
	//}
}

//void ARPGItem::AddEffect(URPGItemEffect* itemEffect)
//{
//	if(itemEffect)
//	{
//		itemEffect->Initialize(ItemOwner);
//		ItemEffects.AddUnique(itemEffect);
//	}
//}

//bool ARPGItem::ConstituionPredicate(const ARPGItem& ip1, const ARPGItem& ip2) const
//{
//	return ip1.Attributes.Constitution < ip2.Attributes.Constitution;
//}