// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "RPG.h"
#include "../Components/RPGAttributeComponent.h"
#include "../Structs/RPGSystemSructs.h"
#include "GameplayTagsManager.h"
#include "GameplayTagsModule.h"
#include "RPGEffectAreaSpread.h"


URPGEffectAreaSpread::URPGEffectAreaSpread(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{

}

void URPGEffectAreaSpread::PreInitialize()
{

}


//Modifier effect is always initialized.
//Because it's only task is to modify other effects, it doesn't care about actors.
bool URPGEffectAreaSpread::Initialize()
{
	IsInitialized = true;
	OnEffectAppiled();
	return true;
}

void URPGEffectAreaSpread::Deinitialize()
{
	OnEffectEnd();
}

//TArray<AActor*> URPGEffectAreaSpread::SpreadEffect()
//{
//	TArray<AActor*> ActorHits;
//	if ((!GetWorld()))
//		return ActorHits;
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
//	return ActorHits;
//}
//
//TArray<AActor*> URPGEffectAreaSpread::SpreadEffectThroughActors(TArray<AActor*> TargetActors)
//{
//	//all actors that have been hit by this spreadl
//	TArray<AActor*> TotalActorHits;
//	if (!GetWorld())
//		return TotalActorHits;
//
//	//EffectSpec.SetEffect();
//
//
//	//make sure we have world
//	float TargetCounter = 0;
//	for (AActor* SpreadFrom : TargetActors)
//	{
//		FCollisionQueryParams SphereParams(this->GetFName(), false, SpreadFrom);
//		TArray<FOverlapResult> Overlaps;
//		GetWorld()->OverlapMulti(Overlaps, SpreadFrom->GetActorLocation(), FQuat::Identity, FCollisionShape::MakeSphere(Radius), SphereParams, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects));
//
//		//actor that have been hit only trough previous actor;
//		TArray<AActor*> ActorHits;
//
//		for (FOverlapResult& overlap : Overlaps)
//		{
//			TotalActorHits.AddUnique(overlap.GetActor());
//			ActorHits.AddUnique(overlap.GetActor());
//		}
//
//		if (ActorHits.Num() > 0)
//		{
//			int32 TargetCounter = 0;
//			if (MaxTargets > TargetCounter)
//			{
//				TargetCounter++;
//				for (AActor* ActorHit : ActorHits)
//				{
//					URPGAttributeComponent* HitActorAttribute = ActorHit->FindComponentByClass<URPGAttributeComponent>();
//
//					if (HitActorAttribute)
//					{
//						//EffectToSpread->SetCauser(CausedBy);
//						EffectToSpread->SetTarget(ActorHit);
//
//						EffectToSpread->PreInitialize();
//						EffectToSpread->Initialize();
//					}
//				}
//
//			}
//		}
//	}
//
//	return TotalActorHits;
//}

UWorld* URPGEffectAreaSpread::GetWorld() const
{
	if (EffectToSpread.IsValid())
	{
		UWorld* const world = GEngine->GetWorldFromContextObject(EffectToSpread.Get());
		return world ? world : NULL;
	}
	return NULL;
}