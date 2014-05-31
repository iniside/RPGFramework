// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Engine.h"
#include "UnrealEd.h"
#include "RPGEngineEditor.generated.h"

UCLASS()
class URPGEngineEditor : public UUnrealEdEngine
{
	GENERATED_UCLASS_BODY()

	//virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar = *GLog) OVERRIDE;

	virtual void Init(IEngineLoop* InEngineLoop) OVERRIDE;
};
