#pragma once
#include "RPG.h"
#include "../Components/RPGAttributeComponent.h"
#include "../Components/RPGEffectManagerComponent.h"
#include "../Components/RPGEffectManagerComponent.h"
#include "RPGComponentBPLibrary.h"

URPGComponentBPLibrary::URPGComponentBPLibrary(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	
}
URPGAttributeComponent* URPGComponentBPLibrary::GetAttributeComponent(AActor* actor)
{
	TArray<URPGAttributeComponent*> attributeComps;
	actor->GetComponents<URPGAttributeComponent>(attributeComps);
	for (URPGAttributeComponent* attrComp : attributeComps)
	{
		return attrComp;
	}
	return NULL;
}

void URPGComponentBPLibrary::ApplyDamage(AActor* Target, AActor* CausedBy, float Damage, TSubclassOf<class URPGDamageType> DamageType)
{
	URPGAttributeComponent* AttrComp = GetAttributeComponent(Target);

	if (AttrComp)
	{
		AttrComp->TakeDamage(Damage, DamageType);
	}
}

