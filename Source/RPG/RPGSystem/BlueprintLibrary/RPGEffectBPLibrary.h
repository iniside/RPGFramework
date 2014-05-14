#pragma once
#include "RPG.h"
#include "../Effects/RPGEffectBase.h"
#include "RPGEffectBPLibrary.generated.h"



UCLASS()
class URPGEffectBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, Category = "RPG|Effect|Static")
		static void ApplyEffect(AActor* target, AActor* causedBy, int32 HowManyToApply, TSubclassOf<class URPGEffectBase> effect);

	UFUNCTION(BlueprintCallable, Category = "RPG|Effect|Static")
		static void ApplyEffectRadial(AActor* CausedBy, FVector HitLocation, float Radius, int32 MaxTargets, TSubclassOf<class URPGEffectBase> effect);

	/*
		Apply effect to all actors in line. Starting from actor that used it.
	*/
	UFUNCTION(BlueprintCallable, Category = "RPG|Effect|Static")
		static void ApplyEffectInLine(AActor* CausedBy, FVector StartLocation, float Lenght, TSubclassOf<class URPGEffectBase> Effect);

};

