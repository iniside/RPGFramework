#pragma once
#include "RPG.h"
#include "RPGCreateNewObject.h"

URPGCreateNewObject::URPGCreateNewObject(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	
}

UObject* URPGCreateNewObject::NewObjectFromBlueprint(UObject* WorldContextObject, TSubclassOf<UObject> UC)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);
	UObject* tempObject = StaticConstructObject(UC);

	return tempObject;
}