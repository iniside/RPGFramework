// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "RPG.h"
#include "GameplayTags.h"
#include "GameplayTagContainer.h"
#include "../Effects/RPGEffectBase.h"
#include "../Effects/RPGEffectInstant.h"
#include "../Effects/RPGEffectPeriodic.h"
#include "../Effects/RPGEffectModifier.h"
#include "RPGSystemSructs.h"


FAttributeSpec& FAttributeSpec::operator=(FAttributeSpec const& Other)
{
	if (this == &Other)
	{
		return *this;
	}
	AttributeModList = Other.AttributeModList;
	OwnedTags.Tags = Other.OwnedTags.Tags;
	RequiredTags.Tags = Other.OwnedTags.Tags;
	return *this;
}

void FEffectModifier::InitializeModifierEffect()
{
	if (ModifierEffect.IsValid())
		return;

	if (ModifierEffectClass)
	{
		ModifierEffect = ConstructObject<URPGEffectModifier>(ModifierEffectClass);
	}
}

bool FEffectModifier::operator==(const FEffectModifier & spec) const
{
	return ModifierEffect == spec.ModifierEffect;
}

void FEffectInstant::InitializeInstantEffect(AActor* Target, AActor* CausedBy)
{
	if (EffectInstant.IsValid())
		return;

	if (EffectInstantClass)
	{
		EffectInstant = ConstructObject<URPGEffectInstant>(EffectInstantClass);
		if (EffectInstant.IsValid())
		{
			EffectInstant->SetTarget(Target);
			EffectInstant->SetCauser(CausedBy);
			//EffectInstant->SetAttributeSpecList(AttributeSpecList);

			//Am I sure I want to do it, this way ?
			EffectInstant->OwnedTags.AppendTags(OwnedTags);

		}
	}
}

bool FEffectInstant::operator==(const FEffectInstant& spec) const
{
	return EffectInstantClass == spec.EffectInstantClass;
}

void FEffectInstant::SetAttributeSpecFromParent(TArray<FAttributeSpec> attrSpec)
{
	//AttributeSpecList = attrSpec;
	//EffectInstant->SetAttributeSpecList(attrSpec);
}

void FEffectSpec::ModifyAttributeSpec(FName AttributeName, float ModifyValue, EAttributeOperation OperationType)
{

}

void FEffectPeriodic::InitializePeriodicEffect(AActor* Target, AActor* CausedBy)
{
	if (EffectPeriod.IsValid())
		return;

	if (EffectPeriodClass)
	{
		EffectPeriod = ConstructObject<URPGEffectPeriodic>(EffectPeriodClass);
		if (EffectPeriod.IsValid())
		{
			EffectPeriod->SetTarget(Target);
			EffectPeriod->SetCauser(CausedBy);

			EffectPeriod->SetPeriodLenght(PeriodLenght);
			EffectPeriod->SetPeriodCount(PeriodCount);

			//Effect->SetAttributeToModify(AttributeToMod);
			//EffectPeriod->SetAttributeSpecList(AttributeSpecList);

		}
	}
}

bool FEffectPeriodic::operator==(const FEffectPeriodic& spec) const
{
	return EffectPeriodClass == spec.EffectPeriodClass;
}

void FEffectCache::CacheInstantEffects(FEffectInstant InstantEffect)
{
	InstantEffectsCache.AddUnique(InstantEffect);
}

void FEffectCache::ModifyCachedInstanEffects(FGameplayTagContainer RequiredTag, FName AttributeName, float ModValue)
{
	for (FEffectInstant& effInst : InstantEffectsCache)
	{
		if (effInst.OwnedTags.HasAnyTag(RequiredTag))
		{
			for (FModdableAttributes& modAttr : effInst.EffectInstant->AttributeSpec.AttributeModList)
			{
				if (modAttr.AttributeName == AttributeName)
				{
					modAttr.ModValue += ModValue;
				}
			}
		}
	}
}

bool FEffectCache::ApplyCachedInstantEffects(AActor* EffectTarget, AActor* EffectCausedBy)
{
	if (InstantEffectsCache.Num() > 0)
	{
		for (FEffectInstant& instEff : InstantEffectsCache)
		{
			if (instEff.EffectInstant->EffectApplicationType == EEffectApplicationType::Effect_Target)
			{
				if (instEff.DoesCauserIsTarget)
				{
					instEff.EffectInstant->SetCauser(EffectCausedBy);
					instEff.EffectInstant->SetTarget(EffectCausedBy);
				}
				else
				{
					instEff.EffectInstant->SetCauser(EffectCausedBy);
					instEff.EffectInstant->SetTarget(EffectTarget);
				}
				instEff.EffectInstant->PreInitialize();
				instEff.EffectInstant->Initialize();
			}
		}
		return true;
	}
	return false;
}

float FEffectCache::ModifyAttributeSpecHelper(float AttributeValue, float ModValue, EAttributeOperation OperationType)
{
	switch (OperationType)
	{
	case EAttributeOperation::Attribute_Add:
		return AttributeValue += ModValue;
	case EAttributeOperation::Attribute_Subtract:
		return AttributeValue -= ModValue;
	case EAttributeOperation::Attribute_Multiply:
		return AttributeValue *= ModValue;
	case EAttributeOperation::Attribute_Divide:
		return AttributeValue /= ModValue;
	case EAttributeOperation::Attribute_Set:
		return AttributeValue = ModValue;
	default:
		return 0;
	}
	return 0;
}