#pragma once
#include "RPG.h"
#include "../Effects/RPGEffectBase.h"
#include "RPGAttributeBPLibrary.generated.h"



UCLASS()
class URPGAttributeBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:

	/*
	Compare Attribute between actors:
	ActorA > ActorB
	*/
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static bool CompareAttributeBetweenActors(AActor* ActorA, AActor* ActorB, FName AttributeName);
};

