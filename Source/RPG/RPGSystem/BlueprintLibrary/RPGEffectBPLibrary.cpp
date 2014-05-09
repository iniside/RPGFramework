#pragma once
#include "RPG.h"
#include "../Effects/RPGEffectBase.h"
#include "../Components/RPGAttributeComponent.h"
#include "RPGEffectBPLibrary.h"

URPGEffectBPLibrary::URPGEffectBPLibrary(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	
}


void URPGEffectBPLibrary::ApplyEffect(AActor* effectTarget, AActor* causedBy, TSubclassOf<class URPGEffectBase> appiledEffect)
{
	if(effectTarget)
	{
		if(appiledEffect)
		{

			TArray<URPGAttributeComponent*> effectMngComps;
			URPGAttributeComponent* effectMngrComp = NULL;
			effectTarget->GetComponents<URPGAttributeComponent>(effectMngComps);
			for (URPGAttributeComponent* effectMngComp : effectMngComps)
			{
				effectMngrComp = effectMngComp;
				break;
			}

			if (effectMngrComp)
			{
				URPGEffectBase* effect = ConstructObject<URPGEffectBase>(appiledEffect);
				effect->SetTarget(effectTarget);
				effect->SetCauser(causedBy);
				effectMngrComp->AddEffect(effect);
			}
			
		}
	}
}
