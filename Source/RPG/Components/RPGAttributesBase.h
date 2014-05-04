#pragma once
#include "RPGAttributesBase.generated.h"

UCLASS(MinimalAPI, const, Blueprintable, BlueprintType)
class URPGAttributesBase : public UObject
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	float HealthProp;
};