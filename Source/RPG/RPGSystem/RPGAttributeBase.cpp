// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "RPG.h"
#include "RPGAttributeBase.h"


float FAttributeProperty::GetNumericValue(class URPGAttributeBase* SourceAttribute)
{
	UNumericProperty* NumericProperty = CastChecked<UNumericProperty>(Attribute);
	void* ValuePtr = NumericProperty->ContainerPtrToValuePtr<void>(SourceAttribute);
	return NumericProperty->GetFloatingPointPropertyValue(ValuePtr);
}

void FAttributeProperty::SetNumericValue(float value, class URPGAttributeBase* TargetAttribute)
{
	UNumericProperty* NumericProperty = CastChecked<UNumericProperty>(Attribute);
	void* ValuePtr = NumericProperty->ContainerPtrToValuePtr<void>(TargetAttribute);
	NumericProperty->SetFloatingPointPropertyValue(ValuePtr, value);
}

URPGAttributeBase::URPGAttributeBase(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	//InitializeAttributesFromDataTable();
}

//void URPGAttributeBase::OnAttributeUpdate()
//{
//	AwsomeTestMultiply = AwsomeTestProperty * 2;
//}
//
//UProperty* URPGAttributeBase::GetAttribute(FName AtributeName, TSubclassOf<URPGAttributeBase> AttributeClass)
//{
//	/*
//	if we have already pointer to property
//	and that property is the same as requested attribute
//	we just return old pointer.
//	*/
//	//if (cachedAttribute)
//	//{
//	//	if (cachedAttribute->GetFName() == AtributeName)
//	//	{
//	//		return cachedAttribute;
//	//	}
//	//}
//	//if ((!cachedAttribute) || cachedAttribute)
//	//{
//	UProperty* temp = NULL;
//	temp = FindFieldChecked<UProperty>(AttributeClass, AtributeName);
//	AttributeProp.SetAttribute(temp);
//		return temp;
//	//}
//return  temp;
//}
//
//float URPGAttributeBase::GetNumericValue(FName AttributeName)
//{
//	UNumericProperty* NumericProperty = CastChecked<UNumericProperty>(GetAttribute(AttributeName, this->GetClass()));
//	void* ValuePtr = NumericProperty->ContainerPtrToValuePtr<void>(this);
//	float tempVal = 0;
//	tempVal = NumericProperty->GetFloatingPointPropertyValue(ValuePtr);
//	return tempVal;
//}
//
//void URPGAttributeBase::SetNumericValue(float value, FName AttributeName)
//{
//	UNumericProperty* NumericProperty = CastChecked<UNumericProperty>(GetAttribute(AttributeName, this->GetClass()));
//	void* ValuePtr = NumericProperty->ContainerPtrToValuePtr<void>(this);
//	NumericProperty->SetFloatingPointPropertyValue(ValuePtr, value);
//}
//
//void URPGAttributeBase::ModifyAttribute(FName AttributeName, float Value, TEnumAsByte<EAttributeOperation> OperationType)
//{
//	float AttributeValue = 0;
//	AttributeValue = GetNumericValue(AttributeName);
//
//	switch (OperationType)
//	{
//		case EAttributeOperation::Attribute_Add:
//		{
//			AttributeValue += Value;
//			SetNumericValue(AttributeValue, AttributeName);
//			return;
//		}
//		case EAttributeOperation::Attribute_Subtract:
//		{
//			AttributeValue -= Value;
//			SetNumericValue(AttributeValue, AttributeName);
//			return;
//		}
//		case EAttributeOperation::Attribute_Multiply:
//		{
//			AttributeValue *= Value;
//			SetNumericValue(AttributeValue, AttributeName);
//			return;
//		}
//		case EAttributeOperation::Attribute_Divide:
//		{
//			AttributeValue = (AttributeValue / Value);
//			SetNumericValue(AttributeValue, AttributeName);
//			return;
//		}
//		case EAttributeOperation::Attribute_Set:
//		{
//			AttributeValue = Value;
//			SetNumericValue(AttributeValue, AttributeName);
//			return;
//		}
//	}
//}
//
//void URPGAttributeBase::InitializeAttributesFromDataTable()
//{
//	if (AttributeDataTable)
//	{
//		static const FString Context = FString(TEXT("URPGAttributeBase::BindDoDataTable"));
//
//		for (TFieldIterator<UProperty> it(GetClass(), EFieldIteratorFlags::IncludeSuper); it; ++it)
//		{
//			UProperty* Prop = *it;
//			UNumericProperty* NumericProperty = Cast<UNumericProperty>(Prop);
//
//			if (NumericProperty)
//			{
//				FString RowNameStr = FString::Printf(TEXT("%s.%s"), *Prop->GetOuter()->GetName(), *Prop->GetName());
//				FAttributeMetaData* MetaData = AttributeDataTable->FindRow<FAttributeMetaData>(FName(*RowNameStr), Context, false);
//				if (MetaData)
//				{
//					void* Data = NumericProperty->ContainerPtrToValuePtr<void>(this);
//					NumericProperty->SetFloatingPointPropertyValue(Data, MetaData->BaseValue);
//				}
//			}
//		}
//	}
//}