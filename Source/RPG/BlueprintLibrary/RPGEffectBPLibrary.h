#pragma once
#include "RPG.h"
#include "../Effects/RPGEffectBase.h"
#include "RPGEffectBPLibrary.generated.h"



UCLASS()
class URPGEffectBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, Category=PowerEffects)
	static void ApplyEffect(AActor* effectTarget, AActor* causedBy, TSubclassOf<class URPGEffectBase> appiledEffect);

	UFUNCTION(BlueprintCallable, Category = PowerEffects)
	static void ApplyEffectTest(ARPGCharacter* effectTarget, ARPGCharacter* causedBy, TSubclassOf<class URPGEffectBase> appiledEffect);

	UFUNCTION(BlueprintCallable, Category = Conditions)
		static void ApplyBleedCondition(ARPGCharacter* conditionTarget, ARPGCharacter* causeBy, TSubclassOf<class URPGConditionBleed> bleed);
	/*
	Ok these two remove functions, are really worthless as they are very generalized, take lots of arguments, and really,
	not doing entirely what I want them to do.
	The probabaly could be used for some reason in character blueprint (idk for what), but they are not specialized enough
	to be really easy to use in Powers blueprints, not to mention in Effects blueprints.
	Though they served well for testing concepts ;)
	*/
	UFUNCTION(BlueprintCallable, Category=PowerEffects)
	static void RemoveSingleEffect(AActor* effectTarget, AActor* causedBy, URPGEffectBase* effectToRemove, TEnumAsByte<EEffectType> effectTypeToRemove, TEnumAsByte<EEffectType> appiledEffectType);

	UFUNCTION(BlueprintCallable, Category=PowerEffects)
	static void RemoveEffects(AActor* effectTarget, TEnumAsByte<EEffectType> appiledEffectType);

	UFUNCTION(BlueprintCallable, Category=PowerEffects)
	static void MultiRadialHitCheck(FHitResult hitData, float radius, TArray<struct FHitResult>& OutHitsResult);

	UFUNCTION(BlueprintCallable, Category=PowerEffects)
	static int32 RemovesEffectStatic(AActor* targetToRemoveEffect, TEnumAsByte<EEffectType> appiledEffectType);
};

