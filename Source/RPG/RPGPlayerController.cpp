// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#include "RPG.h"
#include "RPGPlayerController.h"


ARPGPlayerController::ARPGPlayerController(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	
}

void ARPGPlayerController::BeginPlay()
{
	Super::BeginPlay();
	PlayerCameraManager->ViewPitchMax = 70.0f;
	PlayerCameraManager->ViewPitchMin = -70.0f;
}
void ARPGPlayerController::PrintAllActorsLocations()
{
	//EngineUtils.h
	FActorIterator AllActorsItr = FActorIterator(GetWorld());
	
	//While not reached end (overloaded bool operator)
	while (AllActorsItr)
	{
		ClientMessage(AllActorsItr->GetClass()->GetDesc());
		ClientMessage(AllActorsItr->GetActorLocation().ToString());
		
		//next actor
		++AllActorsItr;
	}
}

//TSubobjectPtr<URPGPowerManagerComponent> ARPGPlayerController::GetCharacterPowerManager()
//{
//	ARPGCharacter* GC = Cast<ARPGCharacter>(GetCharacter());
//	if(GC)
//	{
//		return GC->PowerManager;
//	}
//	return nullptr;
//}