// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "RPG.h"
#include "../Components/RPGAttributeComponent.h"
#include "RPGEffectBase.h"

void FEffectSpec::SetEffect()
{
	if (EffectClass)
	{
		Effect = ConstructObject<URPGEffectBase>(EffectClass);
		if (Effect)
		{
			Effect->SetTarget(Target);
			Effect->SetCauser(CausedBy);
			Effect->SetAttributeToModify(AttributeToMod);
			Effect->SetTargetAttributeSpec(TargetAttributeSpec);
			Effect->SetAttributeSpecList(AttributeSpecList);
		}
	}
}

URPGEffectBase::URPGEffectBase(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	
}
void URPGEffectBase::Tick( float DeltaTime)
{
	//Super::Tick(DeltaTime);

	//currentPeriodTime += DeltaTime;
	//if (currentPeriodTime >= PeriodLenght)
	//{
	//	OnEffectPeriod();
	//	currentPeriodTime = 0;
	//}
	//if (ApplicationType != EApplicationType::Infinite)
	//{
	//	CurrentDuration += DeltaTime;
	//	if (CurrentDuration >= TotalDuration)
	//	{
	//		CurrentDuration = 0;
	//		//Deinitialize();
	//		//OnEffectEnd();
	//		SelfRemoveEffect();
	//	}
	//}
	//OnEffectTick(); //this will tick every frame.
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

void URPGEffectBase::ExecuteTick()
{
	OnEffectPeriod();
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
		
		IsEffectActive = true;
		IsEffectAppiled = true;
		OwnedTags.AddTag(EffectTag);
		//EffectTimerDel.BindDynamic(this, &URPGEffectBase::ExecuteTick);
		OnEffectActivation.Broadcast(this->GetClass(), IsEffectActive);
		OnEffectAppiled();
		return true;
	}
	return false;
}

void URPGEffectBase::Deinitialize()
{
	OnEffectEnd();
	IsEffectActive = false;
	//OnEffectDeactivation.Broadcast(this->GetClass(), IsEffectActive);
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

void URPGEffectBase::SpreadEffect(FEffectSpec EffectSpecs, FVector HitLocation, float Radius, int32 MaxTargets)
{
	if ((!GetWorld()))
		return;

	//EffectSpec.SetEffect();

	FCollisionQueryParams SphereParams(this->GetFName(), false, CausedBy);
	//make sure we have world

	TArray<FOverlapResult> Overlaps;
	GetWorld()->OverlapMulti(Overlaps, HitLocation, FQuat::Identity, FCollisionShape::MakeSphere(Radius), SphereParams, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects));
	//DrawDebugSphere(GetWorld(), HitLocation, Radius, 32, FColor::Black, true, 5.0f);
	//FAttributeRadialEffectEvent RadialEffect;
	//RadialEffect.EffectClass = Effect;
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
					EffectSpecs.Target = HitActor;
					EffectSpecs.CausedBy = CausedBy;
					//EffectSpecs.SetEffect();
					HitActorAttribute->ApplyEffect(EffectSpecs);
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

void URPGEffectBase::ReduceAttributeForDuration(FModdableAttributes AttributeMod, float ReductionTime)
{
	if (TargetAttributes)
	{
		reducedValue = AttributeMod.ModValue;
		TargetAttributes->ModifyAttribute(AttributeMod, EAttributeOperation::Attribute_Subtract);
	}
}

void URPGEffectBase::ModifyTargetAttributes(TArray<FAttributeSpec> AttributesToMod, TEnumAsByte<EAttributeOperation> OperationType)
{
	if (AttributesToModify.Num() > 0)
	{
	}

	for (FAttributeSpec& spec : AttributesToMod)
	{
		if (OwnedTags.HasTag(spec.TargetTag))
		{
			if (TargetAttributes)
			{
				TargetAttributes->ModifyAttributeList(spec.AttributeModList, OperationType);
			}
		}
	}
}

void URPGEffectBase::ModifyAttribute(TArray<FAttributeSpec> AttributeSpec, TEnumAsByte<EAttributeOperation> OperationType)
{
	for (FAttributeSpec& spec : AttributeSpec)
	{
		if (OwnedTags.HasTag(spec.TargetTag))
		{
			if (TargetAttributes)
			{
				TargetAttributes->ModifyAttribute(spec.AttributeMod, OperationType);
			}
		}
	}
}

FModdableAttributes URPGEffectBase::GetModAttribute(FName AttributeName)
{
	FModdableAttributes tempAtt;
	if (AttributesToModify.Num())
	{
		for (int Index = 0; Index < AttributesToModify.Num(); Index++)
		{
			if (AttributesToModify[Index].AttributeName == AttributeName)
			{
				tempAtt.AttributeName = AttributesToModify[Index].AttributeName;
				tempAtt.ModValue = AttributesToModify[Index].ModValue;
				return tempAtt;
			}
		}
	}
	return tempAtt;
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