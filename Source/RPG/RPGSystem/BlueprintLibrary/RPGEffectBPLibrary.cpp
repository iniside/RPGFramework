#pragma once
#include "RPG.h"
#include "../Effects/RPGEffectBase.h"
#include "../Components/RPGAttributeComponent.h"
#include "RPGEffectBPLibrary.h"

URPGEffectBPLibrary::URPGEffectBPLibrary(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	
}


void URPGEffectBPLibrary::ApplyEffect(AActor* target, AActor* causedBy, int32 HowManyToApply, TSubclassOf<class URPGEffectBase> effect)
{
	if (target && causedBy && effect)
	{
		TArray<URPGAttributeComponent*> effectMngComps;
		URPGAttributeComponent* effectMngrComp = NULL;
		target->GetComponents<URPGAttributeComponent>(effectMngComps);
		for (URPGAttributeComponent* effectMngComp : effectMngComps)
		{
			effectMngrComp = effectMngComp;
			break;
		}

		if (effectMngrComp)
		{
			//URPGEffectBase* effectObj = ConstructObject<URPGEffectBase>(effect);
			//effectObj->SetTarget(target);
			//effectObj->SetCauser(causedBy);
			for (int32 Index = 0; Index < HowManyToApply; Index++)
			{
				effectMngrComp->ApplyEffect(target, causedBy, effect);
			}
			//effectObj->AddEffect();
		}
	}
}

void URPGEffectBPLibrary::ApplyEffectRadial(AActor* CausedBy, FVector HitLocation, float Radius, int32 MaxTargets, TSubclassOf<class URPGEffectBase> effect)
{
	if (CausedBy && effect)
	{
		FCollisionQueryParams SphereParams(effect->GetFName(), false, CausedBy);
		UWorld* World = GEngine->GetWorldFromContextObject(CausedBy);
		//make sure we have world
		if (!World)
			return;

		TArray<FOverlapResult> Overlaps;
		World->OverlapMulti(Overlaps, HitLocation ,FQuat::Identity, FCollisionShape::MakeSphere(Radius), SphereParams, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects));
		DrawDebugSphere(World, HitLocation, Radius, 32, FColor::Black, true, 5.0f);
		FAttributeRadialEffectEvent RadialEffect;
		RadialEffect.EffectClass = effect;
		if (Overlaps.Num() >= 0)
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
						HitActorAttribute->ApplyEffect(HitActor, CausedBy, effect);
					}
				}
				
			}
		}
	}
}

void URPGEffectBPLibrary::ApplyEffectInLine(AActor* CausedBy, FVector StartLocation, float Lenght, TSubclassOf<class URPGEffectBase> Effect)
{
	if (CausedBy && Effect)
	{
		FCollisionQueryParams BoxParams(Effect->GetFName(), false, CausedBy);
		UWorld* World = GEngine->GetWorldFromContextObject(CausedBy);
		//make sure we have world
		if (!World)
			return;

		FVector BoxSize = FVector(Lenght, 100.0f, 100.0f);
		FCollisionShape CollisionShapre = FCollisionShape::MakeBox(BoxSize);
		TArray<FOverlapResult> Overlaps;
		//World->OverlapMulti(Overlaps, HitLocation.Location, FQuat::Identity, FCollisionShape::MakeSphere(Radius), SphereParams, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects));
		DrawDebugBox(World, StartLocation, BoxSize, FColor::Red, true, 10.0f);
		//if (Overlaps.Num() > 0)
		//{
		//	//this is going to be very ugly, change it with next version of stable engine!
		//	for (auto It = Overlaps.CreateIterator(); It; ++It)
		//	{
		//		AActor* HitActor = Overlaps[It.GetIndex()].GetActor();

		//		TArray<URPGAttributeComponent*> HitActorAttributes;
		//		URPGAttributeComponent* HitActorAttribute = NULL;
		//		HitActor->GetComponents<URPGAttributeComponent>(HitActorAttributes);
		//		for (URPGAttributeComponent* HitAttr : HitActorAttributes)
		//		{
		//			HitActorAttribute = HitAttr;
		//			break;
		//		}

		//		if (HitActorAttribute)
		//		{
		//			HitActorAttribute->ApplyEffect(HitActor, CausedBy, effect);
		//		}

		//	}
		//}
	}
}