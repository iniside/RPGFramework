#pragma once
#include "RPG.h"
#include "../Effects/RPGEffectBase.h"
#include "../Effects/Conditions/RPGConditionBleed.h"
#include "../RPGCharacter.h"
#include "../Components/RPGEffectManagerComponent.h"
#include "RPGEffectBPLibrary.h"
URPGEffectBPLibrary::URPGEffectBPLibrary(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	
}


void URPGEffectBPLibrary::ApplyEffect(ARPGCharacter* effectTarget, ARPGCharacter* causedBy, TSubclassOf<class URPGEffectBase> appiledEffect)
{
	if(effectTarget)
	{
		if(appiledEffect)
		{
			URPGEffectBase* effect = ConstructObject<URPGEffectBase>(appiledEffect);
			effect->AffectedTarget = effectTarget;
			effect->CausedBy = causedBy;
			effectTarget->EffectManager->AddEffect(effect);
		}
	}
}
void URPGEffectBPLibrary::ApplyEffectTest(ARPGCharacter* effectTarget, ARPGCharacter* causedBy, TSubclassOf<class URPGEffectBase> appiledEffect)
{
	if (effectTarget)
	{
		if (causedBy)
		{
			URPGEffectBase* effect = ConstructObject<URPGEffectBase>(appiledEffect);
			effect->AffectedTarget = effectTarget;
			effect->CausedBy = causedBy;
			effect->AddEffect();
		}
	}
}

void URPGEffectBPLibrary::ApplyBleedCondition(ARPGCharacter* conditionTarget, ARPGCharacter* causeBy, TSubclassOf<class URPGConditionBleed> bleed)
{
	if (conditionTarget)
	{
		if (causeBy)
		{
			if (bleed)
			{
				URPGConditionBleed* bleedObj = ConstructObject<URPGConditionBleed>(bleed);
				bleedObj->SetTarget(conditionTarget);
				bleedObj->SetAppiledBy(causeBy);
				conditionTarget->EffectManager->ConditionBleeds.Add(bleedObj);
			}
		}
	}
}
void URPGEffectBPLibrary::RemoveSingleEffect(AActor* effectTarget, AActor* causedBy, URPGEffectBase* effectToRemove, TEnumAsByte<EEffectType> effectTypeToRemove, TEnumAsByte<EEffectType> appiledEffectType)
{
	ARPGCharacter* GC = Cast<ARPGCharacter>(effectTarget);
	if(GC)
	{
		if(effectToRemove->EffectType == appiledEffectType)
		{
			GC->EffectManager->RemoveEffect(effectToRemove);
		}
	}
}

void URPGEffectBPLibrary::RemoveEffects(AActor* effectTarget, TEnumAsByte<EEffectType> appiledEffectType)
{
	ARPGCharacter* TargetPawn = Cast<ARPGCharacter>(effectTarget);
	if(TargetPawn)
	{
		if(TargetPawn->EffectManager->EffectsList.Num() > 0)
		{
			for(URPGEffectBase* effect : TargetPawn->EffectManager->EffectsList)
			{
				if(effect->EffectType == appiledEffectType)
				{
					TargetPawn->EffectManager->RemoveEffect(effect);
					GLogConsole->Log(FString::FormatAsNumber(GDeltaTime)); //just for debugging don't forget to remove it.
				}
			}
		}
	}
}

void URPGEffectBPLibrary::MultiRadialHitCheck(FHitResult hitData, float radius, TArray<struct FHitResult>& OutHitsResult)
{
	static FName AbilityTag = FName(TEXT("AbilityTrace"));
	UWorld* world = nullptr;
	if(hitData.GetActor())
	{
		world = GEngine->GetWorldFromContextObject(hitData.GetActor());
	}

	TArray<FHitResult> hitList;
	FCollisionShape collShape;
	FCollisionQueryParams queryParam;
	collShape.ShapeType = ECollisionShape::Sphere;
	collShape.SetSphere(radius);

	queryParam.bTraceComplex = true;
	queryParam.TraceTag = AbilityTag;
	queryParam.bTraceAsyncScene = true;

	FVector startLocation = hitData.ImpactPoint;

	DrawDebugSphere(world, startLocation, radius, 32, FColor::Red, false, 10.0f);
	world->SweepMulti(hitList, startLocation, startLocation+radius, FQuat(1.0f, 1.0f, 1.0f, 1.0f), ECollisionChannel::ECC_Pawn, collShape, queryParam);

	if(hitList.Num() > 0)
	{
		OutHitsResult = hitList;
	}
}


int32 URPGEffectBPLibrary::RemovesEffectStatic(AActor* targetToRemoveEffect, TEnumAsByte<EEffectType> appiledEffectType)
{
	int32 effectCount = 0;
	ARPGCharacter* TargetPawn = Cast<ARPGCharacter>(targetToRemoveEffect);
	int32 arrayNum = TargetPawn->EffectManager->EffectsList.Num(); //just checkin remove it!
	if(TargetPawn)
	{
		//use iterator. Array will dynamically change, as elements are removed in this loop (!!).
		//They are also removed outside of loop, when they expire.
		//Iterator will take care for it, for us.
		for(auto it = TargetPawn->EffectManager->EffectsList.CreateIterator(); it; ++it)
		{
			URPGEffectBase* effect  = TargetPawn->EffectManager->EffectsList[it.GetIndex()];
			if(effect->EffectType == appiledEffectType)
			{
				TargetPawn->EffectManager->RemoveEffect(effect); 
				effectCount++;
			}
		}
		return effectCount;
	}
	return effectCount;
}