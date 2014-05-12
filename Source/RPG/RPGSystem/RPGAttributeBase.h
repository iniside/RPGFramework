// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "RPGAttributeBase.generated.h"



USTRUCT(BlueprintType)
struct FAttributeProperty
{
	GENERATED_USTRUCT_BODY()

public:
	FORCEINLINE UProperty* GetAttribute() { return Attribute; };

	FORCEINLINE void SetAttribute(UProperty* NewProperty) { Attribute = NewProperty; };

	FORCEINLINE FName GetName() { return Attribute->GetFName(); };

	float GetNumericValue(class URPGAttributeBase* SourceAttribute);
	void SetNumericValue(float value, class URPGAttributeBase* TargetAttribute);
protected:
	UProperty* Attribute;
};

USTRUCT(BlueprintType)
struct FAttributeMetaData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		float		BaseValue;

	UPROPERTY()
		float		MinValue;

	UPROPERTY()
		float		MaxValue;

	UPROPERTY()
		FString		DerivedAttributeInfo;

	UPROPERTY()
		bool		bCanStack;
};

UCLASS(BlueprintType, Blueprintable, DefaultToInstanced)
class URPGAttributeBase : public UObject
{
	GENERATED_UCLASS_BODY()

	/*virtual void OnAttributeUpdate();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	FAttributeProperty AttributeProp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UDataTable* AttributeDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, EditInline, Category = "Test Property")
		float AwsomeTestProperty;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, EditInline, Category = "Test Property")
		float AwsomeTestMultiply;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	UProperty* GetAttribute(FName AtributeName, TSubclassOf<URPGAttributeBase> AttributeClass);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetNumericValue(FName AttributeName);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void SetNumericValue(float value, FName AttributeName);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void ModifyAttribute(FName AttributeName, float Value, TEnumAsByte<EAttributeOperation> OperationType);

	void InitializeAttributesFromDataTable();

private:
	UProperty* cachedAttribute;*/
};
