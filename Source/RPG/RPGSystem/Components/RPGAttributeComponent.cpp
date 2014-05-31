// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "RPG.h"
#include "../Effects/RPGEffectBase.h"
#include "../Effects/RPGEffectInstant.h"
#include "../Effects/RPGEffectPeriodic.h"
#include "../RPGAttributeBase.h"
#include "RPGAttributeComponent.h"

URPGAttributeComponent::URPGAttributeComponent(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}
void URPGAttributeComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URPGAttributeComponent::InitializeComponent()
{
	Super::InitializeComponent();
}
void URPGAttributeComponent::OnRegister()
{
	Super::OnRegister();
}

void URPGAttributeComponent::TakeDamage(float DamageAmount, FName AttributeName, AActor* CausedBy)
{
	ChangeAttribute(AttributeName, DamageAmount, EAttributeOperation::Attribute_Subtract);
}

void URPGAttributeComponent::ApplyPeriodicEffect(class URPGEffectPeriodic* PeriodicEffect)
{

	//OnRecivedEffect.Broadcast(EffectSpec.Target, EffectSpec.EffectPeriod->GetClass());
	SetPeriodicEffect(PeriodicEffect);
	return;
}

void URPGAttributeComponent::SetPeriodicEffect(class URPGEffectPeriodic* newEffect)
{

	//if effect has been found on array
	//we will check if it have special properties
	//some effects will not be appiled multiple times
	//instead they can have their duration extended
	//or can be reseted. depends on designer choices.
	if (PeriodicEffectsList.Num() > 0)
	{
		URPGEffectPeriodic* firstMatch = NULL;
		for (auto it = PeriodicEffectsList.CreateIterator(); it; ++it)
		{
			URPGEffectPeriodic* match = PeriodicEffectsList[it.GetIndex()];
			if (match->OwnedTags == newEffect->OwnedTags)
			{
				firstMatch = match;
				break;
			}
		}

		if (firstMatch)
		{
			//if effect have stackable duration
			if (firstMatch->StackDuration == true)
			{
				//we simply get duration of current effect on array
				//and add new duration to total pool.
				firstMatch->TotalDuration += newEffect->TotalDuration;
				return;
			}

			/*
			Default behavior
			if the effect is on the list
			we remove it from it
			and then apply it again.
			This way we reset current effect, so we won't end up with multiple effects doing
			the same thing.
			*/
			RemoveEffect(firstMatch);
			PeriodicEffectsList.Add(newEffect);
		}
	}



	//GetWorld()->GetTimerManager().SetTimer(newEffect->EffectTimerDel, newEffect->PeriodLenght, true);

	PeriodicEffectsList.Add(newEffect);
}

void URPGAttributeComponent::ApplyInstantEffects(TArray<FEffectInstant> EffectSpec, class URPGEffectPeriodic* ParentEffect)
{
	/*
	If we have parent effect, this means that incoming effects has been appiled from withing effect.
	Check parent effect if it does have cached version of effects,
	so instead of making new object we will just retrive pointer to it
	*/
	if (CheckParentEffect(ParentEffect))
		return;

	// Cache effects
	for (FEffectInstant& effectSpec : EffectSpec)
	{
		//effectSpec.SetInstantEffect();

		if (!effectSpec.EffectInstant.IsValid())
			return;

		if (!ParentEffect)
			return;

		if (effectSpec.DoesCauserIsTarget)
		{
			//effectSpec.CausedBy = ParentEffect->GetCausedBy();
			//effectSpec.Target = ParentEffect->GetCausedBy();
			effectSpec.EffectInstant->SetTarget(ParentEffect->GetCausedBy());
		}
	}

	//one time action, we should never get here second time.
	//Run cached effects for first time.
	if (CheckParentEffect(ParentEffect))
		return;
}

void URPGAttributeComponent::ApplyInstantEffect(FEffectInstant InstantEffectSpec)
{
	InstantEffectSpec.EffectInstant->PreInitialize();
	InstantEffectSpec.EffectInstant->Initialize();
}

bool URPGAttributeComponent::CheckParentEffect(class URPGEffectPeriodic* ParentEffect)
{
	//if (ParentEffect)
	//{
	//	if (ParentEffect->CachedEffectInstantSpec.Num() > 0)
	//	{
	//		for (FEffectInstant& effect : ParentEffect->CachedEffectInstantSpec)
	//		{
	//			if (effect.EffectInstant.IsValid())
	//			{
	//				effect.EffectInstant->PreInitialize();
	//				effect.EffectInstant->Initialize();
	//				OnRecivedEffect.Broadcast(effect.EffectInstant->GetTarget(), effect.EffectInstant->GetClass());
	//			}
	//		}
	//		return true;
	//	}
	//}
	return false;
}

void URPGAttributeComponent::RemoveEffect(class URPGEffectPeriodic* effectToRemove)
{
	if (effectToRemove)
	{
		effectToRemove->Deinitialize(); //deinitialize effect so it no longer ticks		
		OnEffectRemoved.Broadcast(effectToRemove->GetTarget(), effectToRemove->GetClass());
		int32 element = PeriodicEffectsList.Find(effectToRemove);
		//DestroyEffect(EffectsList[element]);
		PeriodicEffectsList.RemoveSingle(effectToRemove);
	}
}

//void URPGAttributeComponent::DestroyEffect(class URPGEffectPeriodic* EffectToDestroy)
//{
//	if (EffectToDestroy)
//	{
//		if (EffectToDestroy->IsValidLowLevel())
//		{
//			EffectToDestroy->ConditionalBeginDestroy();
//			EffectToDestroy = NULL;
//		}
//	}
//	GetWorld()->ForceGarbageCollection(true);
//}

void URPGAttributeComponent::OnEffectAppiledToMe_Implementation()
{
	//nothing
}

UProperty* URPGAttributeComponent::GetAttribute(FName AtributeName, TSubclassOf<URPGAttributeComponent> AttributeClass)
{
	/*
	if we have already pointer to property
	and that property is the same as requested attribute
	we just return old pointer.
	*/
	if (cachedProperty)
	{
		if (cachedProperty->GetFName() == AtributeName)
		{
			return cachedProperty;
		}
	}
	if ((!cachedProperty) || cachedProperty)
	{
		//UProperty* temp = NULL;
		cachedProperty = FindFieldChecked<UProperty>(AttributeClass, AtributeName);
		//AttributeProp.SetAttribute(cachedProperty;
		return cachedProperty;
	}
	return  cachedProperty;
}

float URPGAttributeComponent::GetNumericValue(FName AttributeName)
{
	if (!AttributeName.IsNone())
	{
		UNumericProperty* NumericProperty = CastChecked<UNumericProperty>(GetAttribute(AttributeName, this->GetClass()));
		void* ValuePtr = NumericProperty->ContainerPtrToValuePtr<void>(this);
		float tempVal = 0;
		tempVal = NumericProperty->GetFloatingPointPropertyValue(ValuePtr);
		return tempVal;
	}
	return 0;
}

void URPGAttributeComponent::SetNumericValue(float value, FName AttributeName)
{
	if (!AttributeName.IsNone())
	{
		UNumericProperty* NumericProperty = CastChecked<UNumericProperty>(GetAttribute(AttributeName, this->GetClass()));
		void* ValuePtr = NumericProperty->ContainerPtrToValuePtr<void>(this);
		NumericProperty->SetFloatingPointPropertyValue(ValuePtr, value);
		OnAttributeChange.Broadcast(AttributeName, value);
	}
}

bool URPGAttributeComponent::IsSmaller(FName AttributeName, float SmallerThan)
{
	float tempValue = GetNumericValue(AttributeName);
	if (tempValue > SmallerThan)
	{
		return false;
	}
	return true;
}

bool URPGAttributeComponent::IsEqual(FName AttributeName, float EqualValue)
{
	float tempValue = GetNumericValue(AttributeName);
	if (tempValue == EqualValue)
	{
		return true;
	}
	return false;
}

bool URPGAttributeComponent::Compare(FName AttributeA, FName AttributeB)
{
	if (GetNumericValue(AttributeA) > GetNumericValue(AttributeB))
		return true;

	return false;
}

void URPGAttributeComponent::ChangeAttribute(FName AttributeName, float ModValue, TEnumAsByte<EAttributeOperation> OperationType)
{
	float AttributeValue = GetNumericValue(AttributeName);
	switch (OperationType)
	{
	case EAttributeOperation::Attribute_Add:
		AttributeValue += ModValue;
		SetNumericValue(AttributeValue, AttributeName);
		return;
	case EAttributeOperation::Attribute_Subtract:
		AttributeValue -= ModValue;
		SetNumericValue(AttributeValue, AttributeName);
		return;
	case EAttributeOperation::Attribute_Multiply:
		AttributeValue *= ModValue;
		SetNumericValue(AttributeValue, AttributeName);
		return;
	case EAttributeOperation::Attribute_Divide:
		AttributeValue = (AttributeValue / ModValue);
		SetNumericValue(AttributeValue, AttributeName);
		return;
	case EAttributeOperation::Attribute_Set:
		AttributeValue = ModValue;
		SetNumericValue(AttributeValue, AttributeName);
		return;
	default:
		return;
	}
}

void URPGAttributeComponent::ModifyAttribute(FModdableAttributes AttributeMod, TEnumAsByte<EAttributeOperation> OperationType)
{
	float AttributeValue = 0;
	AttributeValue = GetNumericValue(AttributeMod.AttributeName);

	switch (OperationType)
	{
	case EAttributeOperation::Attribute_Add:
		AttributeValue += AttributeMod.ModValue;
		SetNumericValue(AttributeValue, AttributeMod.AttributeName);
		return;
	case EAttributeOperation::Attribute_Subtract:
		AttributeValue -= AttributeMod.ModValue;
		SetNumericValue(AttributeValue, AttributeMod.AttributeName);
		return;
	case EAttributeOperation::Attribute_Multiply:
		AttributeValue *= AttributeMod.ModValue;
		SetNumericValue(AttributeValue, AttributeMod.AttributeName);
		return;
	case EAttributeOperation::Attribute_Divide:
		AttributeValue = (AttributeValue / AttributeMod.ModValue);
		SetNumericValue(AttributeValue, AttributeMod.AttributeName);
		return;
	case EAttributeOperation::Attribute_Set:
		AttributeValue = AttributeMod.ModValue;
		SetNumericValue(AttributeValue, AttributeMod.AttributeName);
		return;
	default:
		return;
	}
}

void URPGAttributeComponent::ModifyAttributeList(TArray<FModdableAttributes> Attributes, TEnumAsByte<EAttributeOperation> OperationType)
{
	if (Attributes.Num() > 0)
	{
		for (int32 Index = 0; Index < Attributes.Num(); Index++)
		{
			float AttributeValue = 0;
			AttributeValue = GetNumericValue(Attributes[Index].AttributeName);

			switch (OperationType)
			{
			case EAttributeOperation::Attribute_Add:
				AttributeValue += Attributes[Index].ModValue;
				SetNumericValue(AttributeValue, Attributes[Index].AttributeName);
				break;
			case EAttributeOperation::Attribute_Subtract:
				AttributeValue -= Attributes[Index].ModValue;
				SetNumericValue(AttributeValue, Attributes[Index].AttributeName);
				break;
			case EAttributeOperation::Attribute_Multiply:
				AttributeValue *= Attributes[Index].ModValue;
				SetNumericValue(AttributeValue, Attributes[Index].AttributeName);
				break;
			case EAttributeOperation::Attribute_Divide:
				AttributeValue = (AttributeValue / Attributes[Index].ModValue);
				SetNumericValue(AttributeValue, Attributes[Index].AttributeName);
				break;
			case EAttributeOperation::Attribute_Set:
				AttributeValue = Attributes[Index].ModValue;
				SetNumericValue(AttributeValue, Attributes[Index].AttributeName);
				break;
			default:
				return;
			}
		}
	}
}

float URPGAttributeComponent::AttributeOperation(FName AttributeName, float Value, TEnumAsByte<EAttributeOperation> OperationType)
{

	float AttributeValue = GetNumericValue(AttributeName);

	switch (OperationType)
	{
	case EAttributeOperation::Attribute_Add:
		return AttributeValue += Value;
	case EAttributeOperation::Attribute_Subtract:
		return AttributeValue -= Value;
	case EAttributeOperation::Attribute_Multiply:
		return AttributeValue *= Value;
	case EAttributeOperation::Attribute_Divide:
		return AttributeValue /= Value;
	case EAttributeOperation::Attribute_Set:
		return AttributeValue = Value;
	default:
		return 0;
	}
}

TArray<FModdableAttributes> URPGAttributeComponent::GetAttributeList()
{
	TArray<FModdableAttributes> tempAttributes;

	return tempAttributes;
}