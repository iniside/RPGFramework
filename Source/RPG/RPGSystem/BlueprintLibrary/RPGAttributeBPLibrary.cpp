#pragma once
#include "RPG.h"
#include "../Effects/RPGEffectBase.h"
#include "../Components/RPGAttributeComponent.h"
#include "../Structs/RPGSystemSructs.h"
#include "RPGAttributeBPLibrary.h"

URPGAttributeBPLibrary::URPGAttributeBPLibrary(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	
}


bool URPGAttributeBPLibrary::CompareAttributeBetweenActors(AActor* ActorA, AActor* ActorB, FName AttributeName)
{
	if (ActorA && ActorB)
	{
		/*TArray<URPGAttributeComponent*> ActorAAttributes;
		URPGAttributeComponent* ActorAAttribute = NULL;*/
		URPGAttributeComponent* ActorAAttribute = ActorA->FindComponentByClass<URPGAttributeComponent>();
		//for (URPGAttributeComponent* effectMngComp : ActorAAttributes)
		//{
		//	ActorAAttribute = effectMngComp;
		//	break;
		//}

		//TArray<URPGAttributeComponent*> ActorBAttributes;
		//URPGAttributeComponent* ActorBAttribute = NULL;
		URPGAttributeComponent* ActorBAttribute = ActorB->FindComponentByClass<URPGAttributeComponent>();
		//for (URPGAttributeComponent* effectMngComp : ActorBAttributes)
		//{
		//	ActorBAttribute = effectMngComp;
		//	break;
		//}

		if (ActorAAttribute && ActorBAttribute)
		{
			float ActorAAttributeValue = 0;
			float ActorBAttributeValue = 0;

			ActorAAttributeValue = ActorAAttribute->GetNumericValue(AttributeName);
			ActorBAttributeValue = ActorBAttribute->GetNumericValue(AttributeName);

			if (ActorAAttributeValue > ActorBAttributeValue)
			{
				return true;
			}
			return false;
		}
	}
	return false;
}

TArray<FAttributeSpec> URPGAttributeBPLibrary::ModifyAttributeSpec(TArray<FAttributeSpec> AttributeSpecIn, FName AttributeName, float ModValue, EAttributeOperation OperationType)
{
	if (AttributeSpecIn.Num() > 0)
	{
		for (FAttributeSpec& spec : AttributeSpecIn)
		{
			if (spec.AttributeModList.Num() > 0)
			{
				for (FModdableAttributes& modAtr : spec.AttributeModList)
				{
					//that could moved into seprate function.
					if (modAtr.AttributeName == AttributeName)
					{
						switch (OperationType)
						{
						case EAttributeOperation::Attribute_Add:
							modAtr.ModValue += ModValue;
							break;
						case EAttributeOperation::Attribute_Subtract:
							modAtr.ModValue -= ModValue;
							break;
						case EAttributeOperation::Attribute_Multiply:
							modAtr.ModValue *= ModValue;
							break;
						case EAttributeOperation::Attribute_Divide:
							modAtr.ModValue /= ModValue;
							break;
						case EAttributeOperation::Attribute_Set:
							modAtr.ModValue = ModValue;
								break;
						default:
							break;
						}
					}
				}

			}
			//AttributeSpecOut.SetAttributeSpec(spec);
		}
	}

	return AttributeSpecIn;
}

FEffectSpec URPGAttributeBPLibrary::ModifyEffectSpec(FEffectSpec EffectSpecIn, FGameplayTagContainer RequiredTags, FName AttributeName, float ModValue, EAttributeOperation OperationType)
{
	//for (FAttributeSpec& attrSpec : EffectSpecIn.AttributeSpecList)
	//{
	//	//if (attrSpec.OwnedTags.HasTag(TargetTag))
	//	//{
	//		for (FModdableAttributes& modAtr : attrSpec.AttributeModList)
	//		{
	//			if (modAtr.AttributeName == AttributeName)
	//			{
	//				switch (OperationType)
	//				{
	//				case EAttributeOperation::Attribute_Add:
	//					modAtr.ModValue += ModValue;
	//					break;
	//				case EAttributeOperation::Attribute_Subtract:
	//					modAtr.ModValue -= ModValue;
	//					break;
	//				case EAttributeOperation::Attribute_Multiply:
	//					modAtr.ModValue *= ModValue;
	//					break;
	//				case EAttributeOperation::Attribute_Divide:
	//					modAtr.ModValue /= ModValue;
	//					break;
	//				case EAttributeOperation::Attribute_Set:
	//					modAtr.ModValue = ModValue;
	//					break;
	//				default:
	//					break;
	//				}
	//			}
	//		}
	//	//}
	//}

	return EffectSpecIn;
}