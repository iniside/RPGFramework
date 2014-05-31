// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "RPG.h"
#include "GameplayTagsModule.h"
#include "GameplayTags.h"
#include "RPGEngineEditor.h"


URPGEngineEditor::URPGEngineEditor(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	
}
//bool URPGEngineEditor::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
//{
//	return Super::Exec(InWorld, Cmd, Ar);
//}
void URPGEngineEditor::Init(IEngineLoop* InEngineLoop)
{
	Super::Init(InEngineLoop);
	//DataTable'/Game/Blueprints/MyTags.MyTags'
	//DataTable'/Game/Blueprints/Tags.Tags'
	IGameplayTagsModule& GameplayTagsModule = IGameplayTagsModule::Get();
	GameplayTagsModule.GetGameplayTagsManager().LoadGameplayTagTable("/Game/Blueprints/Tags.Tags");
}
