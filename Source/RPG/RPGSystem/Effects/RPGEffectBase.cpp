// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "RPG.h"
#include "../Components/RPGAttributeComponent.h"
#include "RPGEffectPeriodic.h"
#include "GameplayTagsManager.h"
#include "GameplayTagsModule.h"
#include "RPGEffectBase.h"

URPGEffectBase::URPGEffectBase(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{

}


void URPGEffectBase::PreInitialize()
{

}

bool URPGEffectBase::Initialize()
{
	if (Target)
	{
		URPGAttributeComponent* attributeComp = Target->FindComponentByClass<URPGAttributeComponent>();

		URPGAttributeComponent* CausedByAttribute = CausedBy->FindComponentByClass<URPGAttributeComponent>();

		if (attributeComp && CausedByAttribute)
		{
			TargetAttributes = attributeComp;
			CauserAttributes = CausedByAttribute;
			IsInitialized = true;
			return true;
		}
	}
	return false;
}

void URPGEffectBase::Deinitialize()
{

}

//void URPGEffectBase::SpreadEffect(URPGEffectBase* EffectToSpread, FVector HitLocation, float Radius, int32 MaxTargets)
//{
//	TArray<AActor*> ActorHits;
//	if ((!GetWorld()))
//		return;// ActorHits;
//
//	float TargetCounter = 0;
//	//EffectSpec.SetEffect();
//
//	FCollisionQueryParams SphereParams(this->GetFName(), false, CausedBy);
//	//make sure we have world
//
//	TArray<FOverlapResult> Overlaps;
//	GetWorld()->OverlapMulti(Overlaps, HitLocation, FQuat::Identity, FCollisionShape::MakeSphere(Radius), SphereParams, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects));
//
//
//	for (FOverlapResult& overlap : Overlaps)
//	{
//		ActorHits.AddUnique(overlap.GetActor());
//	}
//
//	if (ActorHits.Num() > 0)
//	{
//		int32 TargetCounter = 0;
//		if (MaxTargets > TargetCounter)
//		{
//			TargetCounter++;
//			for (AActor* ActorHit : ActorHits)
//			{
//				URPGAttributeComponent* HitActorAttribute = ActorHit->FindComponentByClass<URPGAttributeComponent>();
//
//				if (HitActorAttribute)
//				{
//					//EffectToSpread->SetCauser(CausedBy);
//					EffectToSpread->SetTarget(ActorHit);
//
//					EffectToSpread->PreInitialize();
//					EffectToSpread->Initialize();
//				}
//			}
//
//		}
//	}
//
//}

int32 URPGEffectBase::RemoveEffect(FName Tag, int32 Count, class URPGAttributeComponent* TargetToRemoveEffects)
{
	int32 effectCounter = 0;
	int32 effectsRemoved = 0;
	if (TargetToRemoveEffects)
	{
		if (TargetToRemoveEffects->GetEffects().Num() > 0)
		{
			for (auto It = TargetToRemoveEffects->GetEffects().CreateIterator(); It; ++It)
			{
				if (Count >= effectCounter)
				{
					//URPGEffectBase* tempEffect = TargetToRemoveEffects->GetEffects()[It.GetIndex()];
					//if (tempEffect->OwnedTags.HasTag(Tag))
					//{
					//	tempEffect->SelfRemoveEffect();
					//	effectsRemoved++;
					//}
					effectCounter++;
				}
			}
		}
	}
	return effectsRemoved;
}

void URPGEffectBase::ChangeAttribute(TEnumAsByte<EAttributeOperation> OperationType)
{
	TargetAttributes->ChangeAttribute(AttributeName, AttributeValue, OperationType);
}

void URPGEffectBase::ApplyToTarget(FAttributeSpec AttributeSpecIn, TEnumAsByte<EAttributeOperation> OperationType)
{
	for (FModdableAttributes& modAttr : AttributeSpecIn.AttributeModList)
	{
		TargetAttributes->ModifyAttribute(modAttr, OperationType);
	}
}

void URPGEffectBase::ApplyToTargetList(TArray<FModdableAttributes> AttributeMod, TEnumAsByte<EAttributeOperation> OperationType)
{
	for (FModdableAttributes& modAttr : AttributeMod)
	{
		TargetAttributes->ModifyAttribute(modAttr, OperationType);
	}
}

TArray<FModdableAttributes> URPGEffectBase::ModifyAttribute(TArray<FModdableAttributes> AttributeMod, FName AttributeNameIn, float ModifyValue, EAttributeOperation OperationType)
{
	for (FModdableAttributes& modAttr : AttributeMod)
	{
		if (modAttr.AttributeName == AttributeName)
		{
			modAttr.ModValue = ModifyAttributeSpecHelper(ModifyValue, modAttr.ModValue, OperationType);
		}
	}
	return AttributeMod;
}

float URPGEffectBase::ModifyAttributeSpecHelper(float AttributeValue, float ModValue, EAttributeOperation OperationType)
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

float URPGEffectBase::GetTargetAttributeValue(FName AttributeNameIn)
{
	return TargetAttributes->GetNumericValue(AttributeNameIn);
}

float URPGEffectBase::GetCauserAttributeValue(FName AttributeNameIn)
{
	return CauserAttributes->GetNumericValue(AttributeNameIn);
}

TArray < TWeakObjectPtr<URPGEffectBase> > URPGEffectBase::GetModificableEffects()
{
	TArray < TWeakObjectPtr<URPGEffectBase> > tempArray;

	if (TargetAttributes)
	{
		for (auto It = TargetAttributes->GetEffects().CreateIterator(); It; ++It)
		{
			//TWeakObjectPtr<URPGEffectBase> tempEffect = TargetAttributes->GetEffects()[It.GetIndex()];

			//if (tempEffect->OwnedTags.HasAnyTag(OwnedTags))
			//{
			//	tempArray.Add(tempEffect);
			//}
		}
		return tempArray;
	}

	return tempArray;
}

TArray<AActor*> URPGEffectBase::SpreadEffect(URPGEffectBase* EffectIn, FVector FromLoc, float Radius, int32 MaxTargets)
{
	TArray<AActor*> ActorHits;
	if ((!GetWorld()))
		return ActorHits;

	TWeakObjectPtr<URPGEffectPeriodic> PeriodicEffect = Cast<URPGEffectPeriodic>(EffectIn);

	float TargetCounter = 0;
	//EffectSpec.SetEffect();

	FCollisionQueryParams SphereParams(this->GetFName(), false, CausedBy);
	//make sure we have world

	TArray<FOverlapResult> Overlaps;
	GetWorld()->OverlapMulti(Overlaps, FromLoc, FQuat::Identity, FCollisionShape::MakeSphere(Radius), SphereParams, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects));


	for (FOverlapResult& overlap : Overlaps)
	{
		ActorHits.AddUnique(overlap.GetActor());
	}

	if (ActorHits.Num() > 0)
	{
		int32 TargetCounter = 0;
		if (MaxTargets > TargetCounter)
		{
			TargetCounter++;
			for (AActor* ActorHit : ActorHits)
			{
				URPGAttributeComponent* HitActorAttribute = ActorHit->FindComponentByClass<URPGAttributeComponent>();

				if (HitActorAttribute)
				{
					if (PeriodicEffect.IsValid())
					{
						PeriodicEffect->SetTarget(ActorHit);
						PeriodicEffect->PreInitialize();
						PeriodicEffect->Initialize();
						HitActorAttribute->ApplyPeriodicEffect(PeriodicEffect.Get());
						continue;
					}
					EffectIn->SetTarget(ActorHit);

					EffectIn->PreInitialize();
					EffectIn->Initialize();
				}
			}

		}
	}

	return ActorHits;
}

TArray<AActor*> URPGEffectBase::SpreadEffects(TArray<URPGEffectBase*> EffectsIn, FVector FromLoc, float Radius, int32 MaxTargets)
{
	TArray<AActor*> ActorHits;
	if ((!GetWorld()))
		return ActorHits;

	//TWeakObjectPtr<URPGEffectPeriodic> PeriodicEffect = Cast<URPGEffectPeriodic>(EffectIn);

	float TargetCounter = 0;
	//EffectSpec.SetEffect();

	FCollisionQueryParams SphereParams(this->GetFName(), false, CausedBy);
	//make sure we have world

	TArray<FOverlapResult> Overlaps;
	GetWorld()->OverlapMulti(Overlaps, FromLoc, FQuat::Identity, FCollisionShape::MakeSphere(Radius), SphereParams, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects));


	for (FOverlapResult& overlap : Overlaps)
	{
		ActorHits.AddUnique(overlap.GetActor());
	}

	if (ActorHits.Num() > 0)
	{
		int32 TargetCounter = 0;
		if (MaxTargets > TargetCounter)
		{
			TargetCounter++;
			for (AActor* ActorHit : ActorHits)
			{
				URPGAttributeComponent* HitActorAttribute = ActorHit->FindComponentByClass<URPGAttributeComponent>();

				if (HitActorAttribute)
				{
					for (URPGEffectBase* effect : EffectsIn)
					{
						TWeakObjectPtr<URPGEffectPeriodic> periodicEffect = Cast<URPGEffectPeriodic>(effect);

						if (periodicEffect.IsValid())
						{
							periodicEffect->SetTarget(ActorHit);
							periodicEffect->SetCauser(CausedBy);
							periodicEffect->PreInitialize();
							periodicEffect->Initialize();

							HitActorAttribute->ApplyPeriodicEffect(periodicEffect.Get());
							continue;
						}


						effect->SetTarget(ActorHit);
						effect->SetCauser(CausedBy);
						effect->PreInitialize();
						effect->Initialize();
					}
				}
			}

		}
	}

	return ActorHits;
}

UWorld* URPGEffectBase::GetWorld() const
{
	if (Target)
	{
		UWorld* const world = GEngine->GetWorldFromContextObject(Target);
		return world ? world : NULL;
	}
	return NULL;
}