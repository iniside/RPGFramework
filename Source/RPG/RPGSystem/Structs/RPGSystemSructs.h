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

USTRUCT(BlueprintType, Blueprintable)
struct FAttributeSpec
{
	GENERATED_USTRUCT_BODY()

	FAttributeSpec() {}

	/*
		Use this attribute Mod, only with effect that own this tag.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FName TargetTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FModdableAttributes AttributeMod;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	TArray<FModdableAttributes> AttributeModList;
};

USTRUCT(BlueprintType)
struct FEffectScaleData :  public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FEffectScaleData() : ScaleValue(0)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Scale")
	float ScaleValue;
};