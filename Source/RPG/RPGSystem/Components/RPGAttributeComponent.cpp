// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "RPG.h"
#include "../Effects/RPGEffectBase.h"
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

//void URPGAttributeComponent::GetOrCreateAttribute()
//{
//	if (AttributeClass)
//	{
//		if (!AttributeObj)
//		{
//			AttributeObj = ConstructObject<URPGAttributeBase>(AttributeClass);
//		}
//	}
//}


void URPGAttributeComponent::ApplyEffect(AActor* Target, AActor* CausedBy, TSubclassOf<class URPGEffectBase> Effect)
{
	URPGEffectBase* effectTemp = NULL;
	effectTemp = ConstructObject<URPGEffectBase>(Effect);
	if (effectTemp)
	{
		effectTemp->SetTarget(Target);
		effectTemp->SetCauser(CausedBy);
		effectTemp->PreInitialize();

		if (effectTemp->ApplicationType == EApplicationType::InstantApplication)
		{
			OnRecivedEffect.Broadcast(Target, Effect);
			effectTemp->Deinitialize();
		}
		if (effectTemp->Initialize())
		{
			if (effectTemp->ApplicationType == EApplicationType::Periodic)
			{
				//if (CausedBy != GetOwner())
				OnRecivedEffect.Broadcast(Target, Effect);
				SetPeriodicEffect(effectTemp);
			}
		}
	}
}

void URPGAttributeComponent::SetPeriodicEffect(class URPGEffectBase* newEffect)
{

	//if effect has been found on array
	//we will check if it have special properties
	//some effects will not be appiled multiple times
	//instead they can have their duration extended
	//or can be reseted. depends on designer choices.
	if (EffectsList.Num() > 0)
	{
		URPGEffectBase* firstMatch = NULL;
		for (auto it = EffectsList.CreateIterator(); it; ++it)
		{
			URPGEffectBase* match = EffectsList[it.GetIndex()];
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
			EffectsList.Add(newEffect);
		}
	}
	EffectsList.Add(newEffect);
}
void URPGAttributeComponent::RemoveEffect(class URPGEffectBase* effectToRemove)
{
	if (effectToRemove)
	{
		effectToRemove->Deinitialize(); //deinitialize effect so it no longer ticks		
		OnEffectRemoved.Broadcast(effectToRemove->GetTarget(), effectToRemove->GetClass());
		int32 element = EffectsList.Find(effectToRemove);
		//DestroyEffect(EffectsList[element]);
		EffectsList.RemoveSingle(effectToRemove);
	}
}

void URPGAttributeComponent::DestroyEffect(class URPGEffectBase* EffectToDestroy)
{
	if (EffectToDestroy)
	{
		if (EffectToDestroy->IsValidLowLevel())
		{
			EffectToDestroy->ConditionalBeginDestroy();
			EffectToDestroy = NULL;
		}
	}
	GetWorld()->ForceGarbageCollection(true);
}

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
	//if (cachedAttribute)
	//{
	//	if (cachedAttribute->GetFName() == AtributeName)
	//	{
	//		return cachedAttribute;
	//	}
	//}
	//if ((!cachedAttribute) || cachedAttribute)
	//{
	UProperty* temp = NULL;
	temp = FindFieldChecked<UProperty>(AttributeClass, AtributeName);
	//AttributeProp.SetAttribute(temp);
	return temp;
	//}
	return  temp;
}

float URPGAttributeComponent::GetNumericValue(FName AttributeName)
{
	UNumericProperty* NumericProperty = CastChecked<UNumericProperty>(GetAttribute(AttributeName, this->GetClass()));
	void* ValuePtr = NumericProperty->ContainerPtrToValuePtr<void>(this);
	float tempVal = 0;
	tempVal = NumericProperty->GetFloatingPointPropertyValue(ValuePtr);
	return tempVal;
}

void URPGAttributeComponent::SetNumericValue(float value, FName AttributeName)
{
	UNumericProperty* NumericProperty = CastChecked<UNumericProperty>(GetAttribute(AttributeName, this->GetClass()));
	void* ValuePtr = NumericProperty->ContainerPtrToValuePtr<void>(this);
	NumericProperty->SetFloatingPointPropertyValue(ValuePtr, value);
	OnAttributeChange.Broadcast(AttributeName, value);
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

void URPGAttributeComponent::ModifyAttribute(FName AttributeName, float Value, TEnumAsByte<EAttributeOperation> OperationType)
{
	float AttributeValue = 0;
	AttributeValue = GetNumericValue(AttributeName);

	switch (OperationType)
	{
	case EAttributeOperation::Attribute_Add:
		AttributeValue += Value;
		SetNumericValue(AttributeValue, AttributeName);
		return;
	case EAttributeOperation::Attribute_Subtract:
		AttributeValue -= Value;
		SetNumericValue(AttributeValue, AttributeName);
		return;
	case EAttributeOperation::Attribute_Multiply:
		AttributeValue *= Value;
		SetNumericValue(AttributeValue, AttributeName);
		return;
	case EAttributeOperation::Attribute_Divide:
		AttributeValue = (AttributeValue / Value);
		SetNumericValue(AttributeValue, AttributeName);
		return;
	case EAttributeOperation::Attribute_Set:
		AttributeValue = Value;
		SetNumericValue(AttributeValue, AttributeName);
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

TArray<FModdableAttributes> URPGAttributeComponent::GetAttributeList()
{
	TArray<FModdableAttributes> tempAttributes;

	return tempAttributes;
}