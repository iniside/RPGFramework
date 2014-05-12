#pragma once
#include "RPG.h"
#include "../Effects/RPGEffectBase.h"
#include "RPGEffectBPLibrary.generated.h"



UCLASS()
class URPGEffectBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, Category = PowerEffects)
		static void ApplyEffectStatic(AActor* effectTarget, AActor* causedBy, TSubclassOf<class URPGEffectBase> appiledEffect);

	UFUNCTION(BlueprintCallable, Category = "RPG|Effects")
		static void ApplyEffectTest(AActor* target, AActor* causedBy, TSubclassOf<class URPGEffectBase> effect);

	UFUNCTION(BlueprintCallable, Category = "RPG|Effects")
		static void ApplyEffectRadial(AActor* CausedBy, FHitResult HitLocation, float Radius, TSubclassOf<class URPGEffectBase> effect);

};

