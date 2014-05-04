#pragma once
#include "RPG.h"
#include "RPGComponentBPLibrary.generated.h"



UCLASS()
class URPGComponentBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, Category = "RPG|Damage")
	static void ApplyDamage(AActor* Target, AActor* CausedBy, float Damage, TSubclassOf<class URPGDamageType> DamageType);

private:
	static class URPGAttributeComponent* GetAttributeComponent(AActor* actor);
};

