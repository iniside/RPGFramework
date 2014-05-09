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
};

