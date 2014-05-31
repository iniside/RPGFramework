// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "GameplayTags.h"
#include "GameplayTagContainer.h"
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

UENUM()
enum EEffectApplicationType
{
	Effect_ModEffect,
	Effect_Target
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
	
	UPROPERTY(EditAnywhere, Category = "Attributes")
		TEnumAsByte<EAttributeOperation> AttributeOperation;

	FModdableAttributes& operator=(FModdableAttributes const& modAtr)
	{
		if (this == &modAtr)
		{
			return *this;
		}
		AttributeName = modAtr.AttributeName;
		ModValue = modAtr.ModValue;

		return *this;
	}

	void operator+=(FModdableAttributes const& modAtr)
	{
		//if (this == &modAtr)
		//{
		//	return *this;
		//}
		ModValue += modAtr.ModValue;

		//return *this;
	}
};

USTRUCT(BlueprintType, Blueprintable)
struct FAttribute
{
	GENERATED_USTRUCT_BODY()

	FAttribute() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
		FName AttributeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
		float AttributeValue;

	FAttribute& operator=(FAttribute const& Other)
	{
		if (this == &Other)
		{
			return *this;
		}

		AttributeName = Other.AttributeName;
		AttributeValue = Other.AttributeValue;
	}

	bool operator==(const FAttribute& Other) const
	{
		return ((AttributeName == Other.AttributeName) && (AttributeValue == Other.AttributeValue));
	}

	void operator+=(FAttribute const& Other)
	{
		AttributeValue += Other.AttributeValue;
	}
};

USTRUCT(BlueprintType, Blueprintable)
struct FAttributeSpec
{
	GENERATED_USTRUCT_BODY()

	FAttributeSpec() { }

public:
	UPROPERTY(EditAnywhere, Category = "Attribute")
		FGameplayTagContainer OwnedTags;
	/*
	Use this attribute Mod, only with effect that own this tag.
	*/
	UPROPERTY(EditAnywhere, Category = "Attribute")
		FGameplayTagContainer RequiredTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
		TArray<FModdableAttributes> AttributeModList;

	FAttributeSpec& operator=(FAttributeSpec const& Other);

	void operator+=(FAttributeSpec const& Other)
	{
		//if (this == &Other)
		//{
		//	return *this;
		//}
		AttributeModList += Other.AttributeModList;
	}


	void SetAttributeSpec(FAttributeSpec const& Other)
	{
		AttributeModList = Other.AttributeModList;
	}
};

/*
	Select to whom apply effect based on Tags (or something like that).
	When we apply sub effects from one master effects, we need a way to automatically determine who will be target.
	For example in LifeSteal effect, Heal effect target should to be set LifeStealEffect CausedBy actor.
*/
/*
	Add derived struct. Maybe ? That derived struct would contain TSubclassOf<URPGEffectInstant> or Perdiodic, along with
	weak pointer.
	Functions that deal with respectable effects would be easier to deal with, and less prone to make any kind of mistake.
*/
/*
Add struct that will hold effects and their tags. Remove tag from effect itself. I think.
*/
USTRUCT(BlueprintType, Blueprintable)
struct FEffectSpec
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Target")
		bool DoesCauserIsTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
		FGameplayTagContainer OwnedTags;

	void ModifyAttributeSpec(FName AttributeName, float ModifyValue, EAttributeOperation OperationType);
};

USTRUCT(BlueprintType, Blueprintable)
struct FEffectModifier : public FEffectSpec
{
	GENERATED_USTRUCT_BODY()

	FEffectModifier()
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		TSubclassOf<class URPGEffectModifier> ModifierEffectClass;

	UPROPERTY()
		TWeakObjectPtr<class URPGEffectModifier> ModifierEffect;

	void InitializeModifierEffect();
	bool operator==(const FEffectModifier & spec) const;
};

USTRUCT(BlueprintType, Blueprintable)
struct FEffectInstant : public FEffectSpec
{
	GENERATED_USTRUCT_BODY()

	FEffectInstant()
	{}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		TSubclassOf<class URPGEffectInstant> EffectInstantClass;

	UPROPERTY()
	TWeakObjectPtr<class URPGEffectInstant> EffectInstant;

	void InitializeInstantEffect(AActor* Target, AActor* CausedBy);
	void SetAttributeSpecFromParent(TArray<FAttributeSpec> attrSpec);
	bool operator==(const FEffectInstant & spec) const;
};

USTRUCT(BlueprintType, Blueprintable)
struct FEffectPeriodic : public FEffectSpec
{
	GENERATED_USTRUCT_BODY()

	FEffectPeriodic()
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		TSubclassOf<class URPGEffectPeriodic> EffectPeriodClass;

	TWeakObjectPtr<class URPGEffectPeriodic> EffectPeriod;

	UPROPERTY(EditAnywhere, Category = "Duration")
	float PeriodLenght;

	UPROPERTY(EditAnywhere, Category = "Duration")
	float PeriodCount;

	/*
		Effects that going to be automatically cached and appiled by owning periodic effect.
		Specified in EffectPeriodClass.
		How these effects are appiled is up to effect implementation.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Instant Effects")
		TArray<FEffectInstant> InstantEffectsToApply;

	void InitializePeriodicEffect(AActor* Target, AActor* CausedBy);

	bool operator==(const FEffectPeriodic& spec) const;
};

USTRUCT()
struct FEffectCache
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		TArray<FEffectInstant> InstantEffectsCache;

	UPROPERTY()
		TArray<FEffectPeriodic> PeriodicEffectsCache;

	void CacheInstantEffects(FEffectInstant InstantEffect);

	void ModifyCachedInstanEffects(FGameplayTagContainer RequiredTag, FName AttributeName, float ModValue);

	bool ApplyCachedInstantEffects(AActor* EffectTarget, AActor* EffectCausedBy);

private:
	float ModifyAttributeSpecHelper(float AttributeValue, float ModValue, EAttributeOperation OperationType);
};

USTRUCT(BlueprintType, Blueprintable)
struct FEffectInfo
{
	GENERATED_USTRUCT_BODY()

	/*
	My Tags;
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer OwnedTags;

	/*
	Effect that I try to affect must have these tags.
	Check modified effect OwnedTags;
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
		FGameplayTagContainer RequiredTags;

	/*
	Effect with those tags should be ignored by this effect
	Check modified effect OwnedTags;
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
		FGameplayTagContainer IgnoredTags;

};