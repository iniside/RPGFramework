// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "RPG.h"
#include "../Components/RPGAttributeComponent.h"
#include "RPGEffectBase.h"


URPGEffectBase::URPGEffectBase(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	
}
void URPGEffectBase::Tick( float DeltaTime)
{
	//Super::Tick(DeltaTime);

	currentPeriodTime += DeltaTime;
	if (currentPeriodTime >= PeriodLenght)
	{
		OnEffectPeriod();
		currentPeriodTime = 0;
	}
	if (ApplicationType != EApplicationType::Infinite)
	{
		CurrentDuration += DeltaTime;
		if (CurrentDuration >= TotalDuration)
		{
			CurrentDuration = 0;
			//OnEffectEnd();
			SelfRemoveEffect();
		}
	}
	OnEffectTick(); //this will tick every frame.
}
bool URPGEffectBase::IsTickable() const
{
	//UWorld* world = GetCurrentWorld();
	if(IsEffectActive)
	{
		if(Target)
		{
			if(GetWorld())
			{
				//if effect is not periodic there is no point in ticking.
				if (ApplicationType == EApplicationType::Periodic)
				{
					return true;
				}
				return false;
			}
			return false;
		}
		return false;
	}
	return false;
}

TStatId URPGEffectBase::GetStatId() const
{
	return this->GetStatID();
}

void URPGEffectBase::PreInitialize()
{
	if (IsCooldownEffect)
		OwnedTags.AddTag("Ability.Cooldown");
}
//change to to bool
//so we can check if effect has been initialized properly
bool URPGEffectBase::Initialize()
{
	if (Target)
	{
		TArray<URPGAttributeComponent*> effectMngComps;
		URPGAttributeComponent* effectMngrComp = NULL;
		Target->GetComponents<URPGAttributeComponent>(effectMngComps);
		for (URPGAttributeComponent* effectMngComp : effectMngComps)
		{
			effectMngrComp = effectMngComp;
			break;
		}
		TArray<URPGAttributeComponent*> CausedByAttributes;
		URPGAttributeComponent* CausedByAttribute = NULL;
		CausedBy->GetComponents<URPGAttributeComponent>(CausedByAttributes);
		for (URPGAttributeComponent* effectMngComp : effectMngComps)
		{
			CausedByAttribute = effectMngComp;
			break;
		}

		if (effectMngrComp && CausedByAttribute)
		{
			TargetAttributes = effectMngrComp;
			CauserAttributes = CausedByAttribute;
			//targetAttributeComponent = effectMngrComp;
		}
		TotalDuration = PeriodLenght * PeriodsCount;
		OnEffectAppiled();
		IsEffectActive = true;
		IsEffectAppiled = true;
		OnEffectActivation.Broadcast(this->GetClass(), IsEffectActive);
		return true;
	}
	return false;
}

void URPGEffectBase::Deinitialize()
{
	OnEffectEnd();
	IsEffectActive = false;
	OnEffectDeactivation.Broadcast(this->GetClass(), IsEffectActive);
	PeriodLenght = 0;
	TotalDuration = 0;
}

void URPGEffectBase::SelfRemoveEffect()
{
	OnEffectEnd(); //we gice chance to excute any logic before effect is completly removed
	if (ApplicationType != EApplicationType::InstantApplication)
	{
		TargetAttributes->RemoveEffect(this);
	}
}

//void URPGEffectBase::ApplyEffectFromEffect(TSubclassOf<class URPGEffectBase> Effect)
//{
//	if (Target && TargetAttributes)
//	{
//		TargetAttributes->ApplyEffect(Target, Effect);
//	}
//}

bool URPGEffectBase::CanAffectEffect(URPGEffectBase* TargetEffect)
{
	return RequiredTags.HasAnyTag(TargetEffect->OwnedTags);
}

void URPGEffectBase::SpreadEffect(TSubclassOf<class URPGEffectBase> Effect, FVector HitLocation, float Radius, int32 MaxTargets)
{
	if ((!GetWorld()) && (!Effect))
		return;

	FCollisionQueryParams SphereParams(Effect->GetFName(), false, CausedBy);
	//make sure we have world

	TArray<FOverlapResult> Overlaps;
	GetWorld()->OverlapMulti(Overlaps, HitLocation, FQuat::Identity, FCollisionShape::MakeSphere(Radius), SphereParams, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects));
	DrawDebugSphere(GetWorld(), HitLocation, Radius, 32, FColor::Black, true, 5.0f);
	FAttributeRadialEffectEvent RadialEffect;
	RadialEffect.EffectClass = Effect;
	if (Overlaps.Num() > 0)
	{
		//this is going to be very ugly, change it with next version of stable engine!
		int32 TargetCounter = 0;
		for (auto It = Overlaps.CreateIterator(); It; ++It)
		{
			TargetCounter++;
			if (MaxTargets > TargetCounter)
			{
				AActor* HitActor = Overlaps[It.GetIndex()].GetActor();

				TArray<URPGAttributeComponent*> HitActorAttributes;
				URPGAttributeComponent* HitActorAttribute = NULL;
				HitActor->GetComponents<URPGAttributeComponent>(HitActorAttributes);
				for (URPGAttributeComponent* HitAttr : HitActorAttributes)
				{
					HitActorAttribute = HitAttr;
					break;
				}

				if (HitActorAttribute)
				{
					HitActorAttribute->ApplyEffect(HitActor, CausedBy, Effect);
				}
			}

		}
	}
}

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
					URPGEffectBase* tempEffect = TargetToRemoveEffects->GetEffects()[It.GetIndex()];
					if (tempEffect->OwnedTags.HasTag(Tag))
					{
						tempEffect->SelfRemoveEffect();
						effectsRemoved++;
					}
					effectCounter++;
				}
			}
		}
	}
	return effectsRemoved;
}

void URPGEffectBase::ReduceAttributeForDuration(FName Attribute, float ReductionValue, float ReductionTime)
{
	if (TargetAttributes)
	{
		reducedValue = ReductionValue;
		TargetAttributes->ModifyAttribute(Attribute, ReductionValue, EAttributeOperation::Attribute_Subtract);
	}
}

void URPGEffectBase::ModifyTargetAttributes(TArray<FModdableAttributes> AttributesToMod, TEnumAsByte<EAttributeOperation> OperationType)
{
	if (TargetAttributes)
	{
		TargetAttributes->ModifyAttributeList(AttributesToMod, OperationType);
	}
}

TArray < TWeakObjectPtr<URPGEffectBase> > URPGEffectBase::GetModificableEffects()
{
	TArray < TWeakObjectPtr<URPGEffectBase> > tempArray;

	if (TargetAttributes)
	{
		for (auto It = TargetAttributes->GetEffects().CreateIterator(); It; ++It)
		{
			TWeakObjectPtr<URPGEffectBase> tempEffect = TargetAttributes->GetEffects()[It.GetIndex()];

			if (tempEffect->OwnedTags.HasAnyTag(OwnedTags))
			{
				tempArray.Add(tempEffect);
			}
		}
		return tempArray;
	}

	return tempArray;
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