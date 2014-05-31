#pragma once
//#include "RPG.h"
#include "../Effects/RPGEffectBase.h"
#include "../Structs/RPGSystemSructs.h"
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
	UFUNCTION(BlueprintCallable, Category = "Attributes|Static")
	static bool CompareAttributeBetweenActors(AActor* ActorA, AActor* ActorB, FName AttributeName);

	UFUNCTION(BlueprintCallable, Category = "Attributes|Static")
	static TArray<FAttributeSpec> ModifyAttributeSpec(TArray<FAttributeSpec> AttributeSpecIn, FName AttributeName, float ModValue, EAttributeOperation OperationType);
	
	UFUNCTION(BlueprintPure, Category = "Attributes|Static")
		static FEffectSpec ModifyEffectSpec(FEffectSpec EffectSpecIn, FGameplayTagContainer RequiredTags, FName AttributeName, float ModValue, EAttributeOperation OperationType);
};

