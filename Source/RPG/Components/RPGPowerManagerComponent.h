// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "RPG.h"
#include "../Powers/RPGPowerBase.h"
#include "RPGPowerManagerComponent.generated.h"

//DECLARE_DELEGATE_RetVal(URPGPowerBase, FOnNewPowerAdded);
/**
 * 
 */
UCLASS()
class URPGPowerManagerComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

	/* we might want to make it private or protected and do not expose to blueprint
	and only interaction troug equip/remove/get power/s */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=PowerInventory)
	TArray<URPGPowerBase*> PowerList;

	TArray<TWeakObjectPtr<URPGPowerBase>> PowerListSlate;
	/*
	Stub function. Just for testing, probably will need change o removed, do not overuse it!
	*/
	UFUNCTION(BlueprintCallable, Category=PowerManager)
	void AddPower(TSubclassOf<URPGPowerBase> power);

	void DrawInventoryGUI();
};