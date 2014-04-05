#pragma once
//#include "RPG.h"
#include "RPGSharedTypes.generated.h"


USTRUCT(BlueprintType)
struct FLulzStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float AwsomeProperty;
};

UENUM()
enum EConditionType
{
	Burning,
	Bleeding,
	Crippled,
	Frozen,
	Poisoned
};