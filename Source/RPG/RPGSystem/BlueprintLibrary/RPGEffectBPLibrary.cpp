#pragma once
#include "RPG.h"
#include "../Effects/RPGEffectBase.h"
#include "../Effects/RPGEffectInstant.h"
#include "../Effects/RPGEffectPeriodic.h"
#include "../Effects/RPGEffectModifier.h"
#include "../Components/RPGAttributeComponent.h"
#include "RPGEffectBPLibrary.h"

URPGEffectBPLibrary::URPGEffectBPLibrary(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	
}

URPGEffectInstant* URPGEffectBPLibrary::CreateInstantEffect(AActor* EffectTarget, AActor* EffectCausedBy, TSubclassOf<class URPGEffectInstant> InstantEffectIn, TArray<FModdableAttributes> ModAttributesIn, FName AttributeName, float AttributeValue)
{
	TWeakObjectPtr<URPGAttributeComponent> attrComp = EffectTarget->FindComponentByClass<URPGAttributeComponent>();

	if (!attrComp.IsValid())
		return NULL;
	
	TWeakObjectPtr<URPGEffectInstant> tempEff = ConstructObject<URPGEffectInstant>(InstantEffectIn);

	tempEff->SetTarget(EffectTarget);
	tempEff->SetCauser(EffectCausedBy);
	tempEff->AttributesToChange = ModAttributesIn;
	tempEff->AttributeName = AttributeName;
	tempEff->AttributeValue = AttributeValue;
	return tempEff.Get();
}

URPGEffectPeriodic* URPGEffectBPLibrary::CreatePeriodicEffect(AActor* EffectTarget, AActor* EffectCausedBy, TSubclassOf<class URPGEffectPeriodic> PeriodicEffectIn, float PeriodLenght, float PeriodCount)
{
	TWeakObjectPtr<URPGAttributeComponent> attrComp = EffectTarget->FindComponentByClass<URPGAttributeComponent>();

	if (!attrComp.IsValid())
		return NULL;

	TWeakObjectPtr<URPGEffectPeriodic> tempEff = ConstructObject<URPGEffectPeriodic>(PeriodicEffectIn);

	tempEff->SetTarget(EffectTarget);
	tempEff->SetCauser(EffectCausedBy);
	tempEff->SetPeriodCount(PeriodCount);
	tempEff->SetPeriodLenght(PeriodLenght);
	return tempEff.Get();
}

URPGEffectModifier* URPGEffectBPLibrary::CreateModifierEffect(TSubclassOf<class URPGEffectModifier> EffectIn)
{
	TWeakObjectPtr<URPGEffectModifier> tempEff = ConstructObject<URPGEffectModifier>(EffectIn);

	return tempEff.Get();
}

void URPGEffectBPLibrary::Applyffect(class URPGEffectInstant* EffectIn)
{
	if (!EffectIn)
		return;

	EffectIn->PreInitialize();
	EffectIn->Initialize();
}

void URPGEffectBPLibrary::ApplyPeriodicEffect(class URPGEffectPeriodic* EffectIn, TArray<URPGEffectBase*> EffectsToApply)
{
	TWeakObjectPtr<URPGAttributeComponent> attrComp = EffectIn->GetTarget()->FindComponentByClass<URPGAttributeComponent>();
	if (!attrComp.IsValid())
		return;

	EffectIn->EffectsToApply = EffectsToApply;
	EffectIn->PreInitialize();
	EffectIn->Initialize();
	
	attrComp->ApplyPeriodicEffect(EffectIn);
}

URPGEffectInstant* URPGEffectBPLibrary::ModifyEffect(class URPGEffectModifier* ModifierEffect, class URPGEffectInstant* EffectToMod)
{
	if (!ModifierEffect && !EffectToMod)
		return EffectToMod;

	ModifierEffect->AttributeValue = EffectToMod->AttributeValue;
	
	ModifierEffect->PreInitialize();
	ModifierEffect->Initialize();

	EffectToMod->AttributeValue = ModifierEffect->AttributeValue;

	return EffectToMod;
}

void URPGEffectBPLibrary::ApplyInstantEffects(AActor* EffectTarget, AActor* EffectCausedBy, TArray<FEffectInstant> InstantEffectSpecs, TArray<FAttributeSpec> AttributeSpecs)
{
	if (!EffectTarget && !EffectCausedBy)
		return;

	URPGAttributeComponent* attrComp = EffectTarget->FindComponentByClass<URPGAttributeComponent>();

	if (!attrComp)
		return;

	for (FEffectInstant& instEff : InstantEffectSpecs)
	{
		if (instEff.DoesCauserIsTarget)
		{
			instEff.InitializeInstantEffect(EffectCausedBy, EffectCausedBy);
		}
		else
		{
			instEff.InitializeInstantEffect(EffectTarget, EffectCausedBy);
		}

		for (FAttributeSpec& attrSpec : AttributeSpecs)
		{
			if (instEff.EffectInstant->OwnedTags.HasAnyTag(attrSpec.RequiredTags))
			{
				instEff.EffectInstant->AttributeSpec = attrSpec;
			}
		}
		attrComp->ApplyInstantEffect(instEff);
	}
}

void URPGEffectBPLibrary::ApplyEffectPeriodc(AActor* target, AActor* causedBy, int32 HowManyToApply, TSubclassOf<class URPGEffectPeriodic> PeriodicEffectIn)
{
	if (target && causedBy)
	{
		URPGAttributeComponent* attrComp = target->FindComponentByClass<URPGAttributeComponent>();
		TWeakObjectPtr<URPGEffectPeriodic> tempEff = ConstructObject<URPGEffectPeriodic>(PeriodicEffectIn);
		if (!attrComp)
			return;

		tempEff->SetTarget(target);
		tempEff->SetCauser(causedBy);

		//EffectSpec.InitializePeriodicEffect(target, causedBy);
			
			//EffectSpec.EffectPeriod->CacheInstantEffects(instEffect);
		
		tempEff->PreInitialize();
		tempEff->Initialize();

		attrComp->ApplyPeriodicEffect(tempEff.Get());
	}
}

void URPGEffectBPLibrary::ApplyDamage(AActor* Target, AActor* CausedBy, float DamageAmount, FName AttributeName)
{
	TWeakObjectPtr<URPGAttributeComponent> attrComp = Target->FindComponentByClass<URPGAttributeComponent>();

	if (!attrComp.IsValid())
		return;

	attrComp->TakeDamage(DamageAmount, AttributeName, CausedBy);
}

void URPGEffectBPLibrary::LoopTestFunction(TSubclassOf<class URPGEffectBase> EffectIn)
{
	URPGEffectBase* effectTemp;
	for (int i = 0; i < 100000; i++)
	{
		effectTemp = ConstructObject<URPGEffectBase>(EffectIn);
	}
}

void URPGEffectBPLibrary::ApplyEffectRadial(AActor* CausedBy, FVector HitLocation, float Radius, int32 MaxTargets, FEffectSpec EffectSpec)
{
	
}

void URPGEffectBPLibrary::ApplyEffectInLine(AActor* CausedBy, FVector StartLocation, float Lenght, TSubclassOf<class URPGEffectBase> Effect)
{

}

void URPGEffectBPLibrary::ModifyIncomingEffect(FGameplayTagContainer IncomingEffect, FGameplayTagContainer ModifierEffect)
{
	//if (!IncomingEffect.EffectInstant.IsValid() && !ModifierEffect.EffectInstant.IsValid())
	//	return IncomingEffect;

	//IncomingEffect.EffectInstant->AttributeSpec = ModifierEffect.EffectInstant->AttributeSpec;
	
	//return IncomingEffect;
}