#pragma once
#include "RPG.h"
#include "../Effects/RPGEffectBase.h"
#include "RPGEffectBPLibrary.generated.h"



UCLASS()
class URPGEffectBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:
	/** 
		damageAmount - reduce movment speed by X precent (clamped 0..1)
	*/
	UFUNCTION(BlueprintCallable, Category=PowerEffects)
	static void ReduceMovmentSpeed(float damageAmount, float duration, AActor* damageTarget, AActor* causer);

	UFUNCTION(BlueprintCallable, Category=PowerEffects)
	static void ApplyEffect(ARPGCharacter* effectTarget, ARPGCharacter* causedBy, TSubclassOf<class URPGEffectBase> appiledEffect);

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
	static void ApplyTemporaryDamage(AActor* effectTarget, float damage);

	UFUNCTION(BlueprintCallable, Category=PowerEffects)
	static void MultiRadialHitCheck(FHitResult hitData, float radius, TArray<struct FHitResult>& OutHitsResult);

	UFUNCTION(BlueprintCallable, Category=PowerEffects)
	static int32 RemovesEffectStatic(AActor* targetToRemoveEffect, TEnumAsByte<EEffectType> appiledEffectType);
};

