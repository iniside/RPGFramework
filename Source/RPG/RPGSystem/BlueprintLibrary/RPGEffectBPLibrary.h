#pragma once
//#include "RPG.h"
#include "../Effects/RPGEffectBase.h"
#include "RPGEffectBPLibrary.generated.h"



UCLASS()
class URPGEffectBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, Category = "RPG|Effect|Static")
		static class URPGEffectInstant* CreateInstantEffect(AActor* EffectTarget, AActor* EffectCausedBy, TSubclassOf<class URPGEffectInstant> InstantEffectIn, TArray<FModdableAttributes> ModAttributesIn, FName AttributeName, float AttributeValue);

	UFUNCTION(BlueprintCallable, Category = "RPG|Effect|Static")
		static class URPGEffectPeriodic* CreatePeriodicEffect(AActor* EffectTarget, AActor* EffectCausedBy, TSubclassOf<class URPGEffectPeriodic> PeriodicEffectIn, float PeriodLenght, float PeriodCount);
	
	UFUNCTION(BlueprintPure, Category = "RPG|Effect|Static")
		static class URPGEffectModifier* CreateModifierEffect(TSubclassOf<class URPGEffectModifier> EffectIn);

	UFUNCTION(BlueprintCallable, Category = "RPG|Effect|Static")
		static void Applyffect(class URPGEffectInstant* EffectIn);

	UFUNCTION(BlueprintCallable, Category = "RPG|Effect|Static")
		static void ApplyPeriodicEffect(class URPGEffectPeriodic* EffectIn, TArray<URPGEffectBase*> EffectsToApply);

	UFUNCTION(BlueprintPure, Category = "RPG|Effect|Static")
		static class URPGEffectInstant* ModifyEffect(class URPGEffectModifier* ModifierEffect, class URPGEffectInstant* EffectToMod);

	UFUNCTION(BlueprintCallable, Category = "RPG|Effect|Static")
		static void ApplyDamage(AActor* Target, AActor* CausedBy, float DamageAmount, FName AttributeName);

	UFUNCTION(BlueprintCallable, Category = "RPG|Effect|Static")
		static void ApplyInstantEffects(AActor* EffectTarget, AActor* EffectCausedBy, TArray<FEffectInstant> InstantEffectSpecs, TArray<FAttributeSpec> AttributeSpecs);

	UFUNCTION(BlueprintCallable, Category = "RPG|Effect|Static")
		static void LoopTestFunction(TSubclassOf<class URPGEffectBase> EffectIn);

	/*
		target - who will receive effect.
		causedBy - who caused, to target recive effect.
		HowManyToApply - how many effects apply to target.
		EffectSpec - Periodic effect to apply.
		EffectInstantSpec - instant effects that will appiled by periodic effect.
		AttributeSpec - Attributes that will be modified by periodic and instant effects.
	*/
	UFUNCTION(BlueprintCallable, Category = "RPG|Effect|Static")
		static void ApplyEffectPeriodc(AActor* target, AActor* causedBy, int32 HowManyToApply, TSubclassOf<class URPGEffectPeriodic> PeriodicEffectIn);

	UFUNCTION(BlueprintCallable, Category = "RPG|Effect|Static")
		static void ApplyEffectRadial(AActor* CausedBy, FVector HitLocation, float Radius, int32 MaxTargets, FEffectSpec EffectSpec);

	/*
		Apply effect to all actors in line. Starting from actor that used it.
	*/
	UFUNCTION(BlueprintCallable, Category = "RPG|Effect|Static")
		static void ApplyEffectInLine(AActor* CausedBy, FVector StartLocation, float Lenght, TSubclassOf<class URPGEffectBase> Effect);


	UFUNCTION(BlueprintCallable, Category = "RPG|Effect|Static")
		static void ModifyIncomingEffect(FGameplayTagContainer IncomingEffect, FGameplayTagContainer ModifierEffect);
};

