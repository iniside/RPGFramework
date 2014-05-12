#pragma once
#include "RPG.h"
#include "../Effects/RPGEffectBase.h"
#include "../Components/RPGAttributeComponent.h"
#include "RPGAttributeBPLibrary.h"

URPGAttributeBPLibrary::URPGAttributeBPLibrary(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	
}


bool URPGAttributeBPLibrary::CompareAttributeBetweenActors(AActor* ActorA, AActor* ActorB, FName AttributeName)
{
	if (ActorA && ActorB)
	{
		TArray<URPGAttributeComponent*> ActorAAttributes;
		URPGAttributeComponent* ActorAAttribute = NULL;
		ActorA->GetComponents<URPGAttributeComponent>(ActorAAttributes);
		for (URPGAttributeComponent* effectMngComp : ActorAAttributes)
		{
			ActorAAttribute = effectMngComp;
			break;
		}

		TArray<URPGAttributeComponent*> ActorBAttributes;
		URPGAttributeComponent* ActorBAttribute = NULL;
		ActorB->GetComponents<URPGAttributeComponent>(ActorBAttributes);
		for (URPGAttributeComponent* effectMngComp : ActorBAttributes)
		{
			ActorBAttribute = effectMngComp;
			break;
		}

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
