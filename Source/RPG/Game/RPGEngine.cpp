// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "RPG.h"
#include "GameplayTagsModule.h"
#include "GameplayTags.h"
#include "RPGEngine.h"


URPGEngine::URPGEngine(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	
}

void URPGEngine::Init(IEngineLoop* InEngineLoop)
{
	Super::Init(InEngineLoop);

	IGameplayTagsModule& GameplayTagsModule = IGameplayTagsModule::Get();
	GameplayTagsModule.GetGameplayTagsManager().LoadGameplayTagTable("/Game/Blueprints/Tags.Tags");
}
