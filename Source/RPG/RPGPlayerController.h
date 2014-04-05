// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "RPGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ARPGPlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()

	void PrintAllActorsLocations();
	
	//TSubobjectPtr<URPGPowerManagerComponent> GetCharacterPowerManager();
};
