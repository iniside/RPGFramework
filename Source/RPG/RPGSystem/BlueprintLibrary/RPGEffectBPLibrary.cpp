#pragma once
#include "RPG.h"
#include "../Effects/RPGEffectBase.h"
#include "../Components/RPGAttributeComponent.h"
#include "RPGEffectBPLibrary.h"

URPGEffectBPLibrary::URPGEffectBPLibrary(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	
}

void URPGEffectBPLibrary::ApplyEffectStatic(AActor* effectTarget, AActor* causedBy, TSubclassOf<class URPGEffectBase> appiledEffect)
{
	//if(effectTarget)
	//{
	//	if(appiledEffect)
	//	{

	//		TArray<URPGAttributeComponent*> effectMngComps;
	//		URPGAttributeComponent* effectMngrComp = NULL;
	//		effectTarget->GetComponents<URPGAttributeComponent>(effectMngComps);
	//		for (URPGAttributeComponent* effectMngComp : effectMngComps)
	//		{
	//			effectMngrComp = effectMngComp;
	//			break;
	//		}

	//		if (effectMngrComp)
	//		{
	//			URPGEffectBase* effect = ConstructObject<URPGEffectBase>(appiledEffect);
	//			effect->SetTarget(effectTarget);
	//			effect->SetCauser(causedBy);
	//			effectMngrComp->AddEffect(effect);
	//		}
	//		
	//	}
	//}
}

void URPGEffectBPLibrary::ApplyEffectTest(AActor* target, AActor* causedBy, TSubclassOf<class URPGEffectBase> effect)
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
			effectMngrComp->ApplyEffect(target, causedBy, effect);
			//effectObj->AddEffect();
		}
	}
}

void URPGEffectBPLibrary::ApplyEffectRadial(AActor* CausedBy, FHitResult HitLocation, float Radius, TSubclassOf<class URPGEffectBase> effect)
{
	if (CausedBy && effect)
	{
		FCollisionQueryParams SphereParams(effect->GetFName(), false, CausedBy);
		UWorld* World = GEngine->GetWorldFromContextObject(CausedBy);
		//make sure we have world
		if (!World)
			return;

		TArray<FOverlapResult> Overlaps;
		World->OverlapMulti(Overlaps, HitLocation.Location ,FQuat::Identity, FCollisionShape::MakeSphere(Radius), SphereParams, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects));
		DrawDebugSphere(World, HitLocation.Location, Radius, 32, FColor::Black, true, 5.0f);
		if (Overlaps.Num() > 0)
		{
			//this is going to be very ugly, change it with next version of stable engine!
			for (auto It = Overlaps.CreateIterator(); It; ++It)
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
