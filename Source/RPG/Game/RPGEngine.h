// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Engine.h"
#include "RPGEngine.generated.h"

UCLASS()
class URPGEngine : public UGameEngine
{
	GENERATED_UCLASS_BODY()

	virtual void Init(IEngineLoop* InEngineLoop) OVERRIDE;
};
