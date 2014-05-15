// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Object.h"
#include "Tickable.h"
#include "GameplayTagContainer.h"
#include "../Structs/RPGSystemSructs.h"
#include "RPGEffectBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEffectActivation, TSubclassOf<URPGEffectBase>, EffectClass, bool, IsEffectActive);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEffectDeactivation, TSubclassOf<URPGEffectBase>, EffectClass, bool, IsEffectActive);
UENUM()
enum EApplicationType
{
	InstantApplication,
	Periodic,
	Infinite
};

USTRUCT(BlueprintType, Blueprintable)
struct FEffectSpec
{
	GENERATED_USTRUCT_BODY()

	FEffectSpec()
	{}

	class URPGAttributeBase* TargetAttribute;
	
	AActor* Target;

	AActor* CausedBy;
	
	/*
		In case we want to modify multiple Attributes in single shot.
	*/
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	//TArray<FModdableAttributes> AttributesToMod;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	TArray<FAttributeSpec> AttributeSpecList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	TSubclassOf<class URPGEffectBase> EffectClass;

	/*
		Helper in case we want to modify only single attribute.
		More convinient than extrating single entry from array.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FModdableAttributes AttributeToMod;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FAttributeSpec TargetAttributeSpec;

	void SetEffect();

	class URPGEffectBase* Effect;
};

USTRUCT(BlueprintType, Blueprintable)
struct FEffectCollection
{
	GENERATED_USTRUCT_BODY()

	FEffectCollection() {}

	TArray<FEffectSpec> EffectSpecs;
};

UENUM()
enum EEffectDuration
{
	InfiniteDuration,
	Period
};

UCLASS(Blueprintable, BlueprintType, MinimalAPI)
class URPGEffectBase : public UObject ,public FTickableGameObject
{
	GENERATED_UCLASS_BODY()

	virtual void Tick(float DeltaTime)  OVERRIDE;
	virtual bool IsTickable() const OVERRIDE;
	virtual TStatId GetStatId() const OVERRIDE;

public:
	//it's hack for testing, don't use it!
	UPROPERTY(EditAnywhere, Category = "Effect Type")
		bool IsCooldownEffect;

	UPROPERTY()
	float TotalDuration;

	UPROPERTY(BlueprintReadOnly, Category = "RPG|Effect")
	float CurrentDuration;

	UPROPERTY(EditAnywhere, Category = "Base Properties")
	bool StackDuration;

	UPROPERTY(EditAnywhere, Category = "Periodic Effect")
	float PeriodLenght;

	UPROPERTY(EditAnywhere, Category = "Periodic Effect")
	float PeriodsCount;

	UPROPERTY(EditAnywhere, Category = "Base Properties")
	TEnumAsByte<EApplicationType> ApplicationType;

	UPROPERTY(EditAnywhere, Category = "BaseProperties")
		FName EffectTag;

	/*
		My Tags;
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer OwnedTags;

	/*
		Effect that I try to affect must have these tags.
		Check modified effect OwnedTags;
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer RequiredTags;

	/*
		Effect with those tags should be ignored by this effect
		Check modified effect OwnedTags;
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer IgnoredTags;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "RPG|Effect")
	FOnEffectActivation OnEffectActivation;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "RPG|Effect")
	FOnEffectDeactivation OnEffectDeactivation;
	/*
		Check if TargetEffect Owns any of RequiredTags.
	*/
	bool CanAffectEffect(URPGEffectBase* TargetEffect);
	
	UFUNCTION()
	void ExecuteTick();

	FTimerDynamicDelegate EffectTimerDel;
protected:
	/**
	Target affected by appiled effect
	*/
	UPROPERTY(BlueprintReadWrite, Category=BaseProperties)
	AActor* Target; //maybe change it to RPGCharacter ?

	/**
	Who appiled effect to target.
	*/
	UPROPERTY(BlueprintReadWrite, Category=BaseProperties)
	AActor* CausedBy;

	UPROPERTY()
	bool IsEffectAppiled;
	
	UPROPERTY()
	bool IsEffectActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	TArray<FModdableAttributes> AttributesToModify;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FModdableAttributes AttributeToModify;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FAttributeSpec TargetAttributeSpec;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		TArray<FAttributeSpec> AttributeSpecList;

	UPROPERTY(BlueprintReadOnly, Category="Attributes")
	class URPGAttributeComponent* TargetAttributes;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	class URPGAttributeComponent* CauserAttributes;

public:
	FORCEINLINE void SetTarget(AActor* target) { Target = target; };
	FORCEINLINE AActor* GetTarget() { return Target; };
	FORCEINLINE void SetCauser(AActor* causer) { CausedBy = causer; };
	FORCEINLINE void SetAttributesToModify(TArray<FModdableAttributes> attributesToMod){ AttributesToModify = attributesToMod; };
	FORCEINLINE void SetAttributeToModify(FModdableAttributes attributeToMod){ AttributeToModify = attributeToMod; };
	FORCEINLINE TArray<FModdableAttributes> GetModdableAttributes() { return AttributesToModify; };
	FORCEINLINE void SetTargetAttributeSpec(FAttributeSpec attributeSpec) { TargetAttributeSpec = attributeSpec; };
	FORCEINLINE void SetAttributeSpecList(TArray<FAttributeSpec> specList){ AttributeSpecList = specList; };
	//FORCEINLINE void SetTargetAttributes(URPGAttributeBase* attributes) { CausedBy = causer; };

	/** 
	 Do preinitialization taks. Like assign tags.
	 Anything that do not invovlve Target or CausedBy can be setup here.
	*/
	virtual void PreInitialize();
	virtual bool Initialize();

	/** 
		Deinitialaize effect, effecticly setting it all ticking properties to 0 or false
	*/
	virtual void Deinitialize();

	/*
		Allow to spread another effect from effect in radius around where the effect is active.
		Use it instead of static version, for safety and predictable results!
	*/
	UFUNCTION(BlueprintCallable, Category = "RPG|Effect")
		void SpreadEffect(FEffectSpec EffectSpecs, FVector HitLocation, float Radius, int32 MaxTargets);
	
	/*
		Function remove effects as "frist come, first serve"
		Return how many effects has been removed;
	*/
	UFUNCTION(BlueprintCallable, Category = "RPG|Effect")
		int32 RemoveEffect(FName Tag, int32 Count, class URPGAttributeComponent* TargetToRemoveEffects);

	/*
		Attribute - Name of attribute to reduce
		ReductionValue - By how much reduce attribute
		ReductionTime - if 0 it will reduce for duration of effect (Default)
	*/
	UFUNCTION(BlueprintCallable, Category = "RPG|Effect")
		void ReduceAttributeForDuration(FModdableAttributes AttributeMod, float ReductionTime = 0);
private:
		float reducedValue;
public:
	UFUNCTION(BlueprintCallable, Category = "RPG|Effect")
		void ModifyTargetAttributes(TArray<FAttributeSpec> AttributesToMod, TEnumAsByte<EAttributeOperation> OperationType);

	UFUNCTION(BlueprintCallable, Category = "RPG|Effect")
		void ModifyAttribute(TArray<FAttributeSpec> AttributeSpec, TEnumAsByte<EAttributeOperation> OperationType);

	UFUNCTION(BlueprintCallable, Category = "RPG|Effect")
		FModdableAttributes GetModAttribute(FName AttributeName);

	/*
		Get Array of effects from target, which Owns any of Required Tags.
		These effects can be modified, though dunno how...
	*/
	TArray < TWeakObjectPtr<URPGEffectBase> > GetModificableEffects();
	//bool CanAffectOtherEffect()

	//UPROPERTY(BlueprintAssignable)
	//FEffectAppiledToTarget OnEffectAppiledtoTarget;

	UFUNCTION(BlueprintImplementableEvent, Category = "RPG|Effect")
	void OnEffectAppiled();

	UFUNCTION(BlueprintImplementableEvent, Category = "RPG|Effect")
	void OnEffectEnd();

	UFUNCTION(BlueprintImplementableEvent, Category = "RPG|Effect")
	void OnEffectPeriod();

	UFUNCTION(BlueprintImplementableEvent, Category = "RPG|Effect")
	void OnValueChanged();

	UFUNCTION(BlueprintImplementableEvent, Category = "RPG|Effect")
	void OnEffectTick();

	//UFUNCTION(BlueprintCallable, Category = "Effects")
	//	void ApplyEffectFromEffect(TSubclassOf<class URPGEffectBase> Effect);

protected:
	void SelfRemoveEffect();
public:
	float currentPeriodTime;
	float OriginalConstitution;
	float AttributeDecrease;
	float MaxHealthDrain;
	//class URPGAttributeComponent* targetAttributeComponent;
protected:
	virtual class UWorld* GetWorld() const OVERRIDE;
};
