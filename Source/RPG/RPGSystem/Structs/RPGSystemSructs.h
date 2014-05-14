// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "RPGSystemSructs.generated.h"

UENUM()
enum EAttributeOperation
{
	Attribute_Add,
	Attribute_Subtract,
	Attribute_Multiply,
	Attribute_Divide,
	Attribute_Set
};

USTRUCT(BlueprintType, Blueprintable)
struct FModdableAttributes
{
	GENERATED_USTRUCT_BODY()

	FModdableAttributes() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FName AttributeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		float ModValue;
};