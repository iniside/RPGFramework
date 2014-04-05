// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "RPG.h"
#include "RPGPlayerController.h"
#include "RPGGameMode.h"

ARPGGameMode::ARPGGameMode(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	PlayerControllerClass = ARPGPlayerController::StaticClass();
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FObjectFinder<UClass> PlayerPawnBPClass(TEXT("Class'/Game/Blueprints/MyCharacter.MyCharacter_C'"));
	static ConstructorHelpers::FObjectFinder<UBlueprint> TheHUDOb(TEXT("Blueprint'/Game/Blueprints/HUD/BP_RPGHUD.BP_RPGHUD'"));
	if (PlayerPawnBPClass.Object != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Object;

	}
	if (TheHUDOb.Object != NULL)
	{
		HUDClass = (UClass*)TheHUDOb.Object->GeneratedClass;
	}
}
