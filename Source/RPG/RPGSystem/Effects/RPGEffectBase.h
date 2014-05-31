// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Object.h"
#include "Tickable.h"
#include "GameplayTagContainer.h"
#include "../Structs/RPGSystemSructs.h"
#include "RPGEffectBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEffectActivation, TSubclassOf<URPGEffectBase>, EffectClass, bool, IsEffectActive);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEffectDeactivation, TSubclassOf<URPGEffectBase>, EffectClass, bool, IsEffectActive);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEffectPeriod);
UENUM()
enum EApplicationType
{
	InstantApplication,
	Periodic,
	Infinite
};

UENUM()
enum EEffectDuration
{
	InfiniteDuration,
	Period
};

UCLASS(Blueprintable, BlueprintType, MinimalAPI)
class URPGEffectBase : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Base Properties")
		TEnumAsByte<EApplicationType> ApplicationType;

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

	UPROPERTY(EditAnywhere, Category = "Effect Type")
		TEnumAsByte<EEffectApplicationType> EffectApplicationType;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "RPG|Effect")
		FOnEffectActivation OnEffectActivation;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "RPG|Effect")
		FOnEffectDeactivation OnEffectDeactivation;

//make it protected. Later. Maybe.
	UPROPERTY(BlueprintReadWrite, Category = "Attributes")
		FAttributeSpec AttributeSpec;

	UPROPERTY(BlueprintReadWrite, Category = "Attributes")
		TArray<FModdableAttributes> AttributesToChange;

	/*
		Attributes
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FName AttributeName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		float AttributeValue;

	UPROPERTY(BlueprintReadOnly, Category = "Effects")
		TArray<URPGEffectBase*> EffectsToApply;
protected:
	/**
	Target affected by appiled effect
	*/
	UPROPERTY(BlueprintReadWrite, Category= "Actors")
	AActor* Target; //maybe change it to RPGCharacter ?

	/**
	Who appiled effect to target.
	*/
	UPROPERTY(BlueprintReadWrite, Category= "Actors")
	AActor* CausedBy;
	
	/*
		Who originally appiled effect.
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Actors")
		AActor* Instigator;

	UPROPERTY()
	bool IsEffectActive;

	UPROPERTY(BlueprintReadOnly, Category="Attributes")
	class URPGAttributeComponent* TargetAttributes;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	class URPGAttributeComponent* CauserAttributes;

	UPROPERTY()
	bool IsInitialized;

public:
	FORCEINLINE void SetTarget(AActor* target) { Target = target; };
	FORCEINLINE AActor* GetTarget() { return Target; };
	
	FORCEINLINE void SetCauser(AActor* causer) { CausedBy = causer; };
	FORCEINLINE AActor* GetCausedBy() { return CausedBy; };

	FORCEINLINE bool GetIsInitialized() { return IsInitialized; };

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
	//UFUNCTION(BlueprintCallable, Category = "RPG|Effect")
	//	void SpreadEffect(URPGEffectBase* EffectToSpread, FVector HitLocation, float Radius, int32 MaxTargets);
	/*
		Function remove effects as "frist come, first serve"
		Return how many effects has been removed;
	*/
	UFUNCTION(BlueprintCallable, Category = "RPG|Effect")
		int32 RemoveEffect(FName Tag, int32 Count, class URPGAttributeComponent* TargetToRemoveEffects);

public:
	UFUNCTION(BlueprintCallable, Category = "RPG|Effect")
		void ChangeAttribute(TEnumAsByte<EAttributeOperation> OperationType);

	UFUNCTION(BlueprintCallable, Category = "RPG|Effect")
		void ApplyToTarget(FAttributeSpec AttributeSpecIn, TEnumAsByte<EAttributeOperation> OperationType);
	
	UFUNCTION(BlueprintCallable, Category = "RPG|Effect")
		void ApplyToTargetList(TArray<FModdableAttributes> AttributeMod, TEnumAsByte<EAttributeOperation> OperationType);

	UFUNCTION(BlueprintPure, Category = "RPG|Effect")
		TArray<FModdableAttributes> ModifyAttribute(TArray<FModdableAttributes> AttributeMod, FName AttributeNameIn, float ModifyValue, EAttributeOperation OperationType);

	UFUNCTION(BlueprintCallable, Category = "RPG|Effect")
		TArray<AActor*> SpreadEffect(URPGEffectBase* EffectIn, FVector FromLoc, float Radius, int32 MaxTargets);

	UFUNCTION(BlueprintCallable, Category = "RPG|Effect")
		TArray<AActor*> SpreadEffects(TArray<URPGEffectBase*> EffectsIn, FVector FromLoc, float Radius, int32 MaxTargets);

	/*
		Easier access, for polling target attribute.
	*/
	UFUNCTION(BlueprintPure, Category = "RPG|Effect")
		float GetTargetAttributeValue(FName AttributeNameIn);

	UFUNCTION(BlueprintPure, Category = "RPG|Effect")
		float GetCauserAttributeValue(FName AttributeNameIn);

protected:
	float ModifyAttributeSpecHelper(float AttributeValue, float ModValue, EAttributeOperation OperationType);
public:
	/*
		Get Array of effects from target, which Owns any of Required Tags.
		These effects can be modified, though dunno how...
	*/
	TArray < TWeakObjectPtr<URPGEffectBase> > GetModificableEffects();

	UFUNCTION(BlueprintImplementableEvent, Category = "RPG|Effect")
		void OnEffectPreInitialized();

	UFUNCTION(BlueprintImplementableEvent, Category = "RPG|Effect")
		void OnEffectInitialized();

	UFUNCTION(BlueprintImplementableEvent, Category = "RPG|Effect")
		void OnEffectPostInitialized();

	UFUNCTION(BlueprintImplementableEvent, Category = "RPG|Effect")
		void OnEffectDeinitialized();

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

protected:
	virtual class UWorld* GetWorld() const OVERRIDE;
};
