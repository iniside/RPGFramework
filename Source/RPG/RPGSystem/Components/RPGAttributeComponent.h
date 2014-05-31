// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
//#include "RPG.h"
#include "GameplayTagContainer.h"
#include "../Structs/RPGSystemSructs.h"
#include "RPGAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRecievedEffect, class AActor*, WhoRecievedEffect, TSubclassOf<class URPGEffectBase>, Effect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChange, FName, AttributeName, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEffectRemoved, class AActor*, EffectTarget, TSubclassOf<class URPGEffectBase>, Effect);
//DECLARE_DELEGATE_RetVal(URPGPowerBase, FOnNewPowerAdded);
/**
 * Idea for this, come from Epic source (you can look it Runtime/Skillsystem).
 * It is supposed to completly decouple attributes from actor, and by extensions
 * decouple things like Effects, Powers, Deadth from pawn as well.
 * all those things should just check if actor have this component.
 * if it doesn't they simply ignore actor, as they can't affect it.
 * Things to consider:
 * Damage handling - I still wan't to make use of Unreal systems. Like TakeDamage, and DamgeType
 * I can't see how I could use TakeDamage, without coupling again component to Pawn
 * so component will probably need to have it's own Damage handling logic, along with events
 * and blueprint functions.
 * Damage types can be probably still levelraged, to calculate damage
 * but that would mean that all damage types with RPG* would be coupled to this component
 * and attributes it contain. Not very flexible system, but greatly simplify design.
 * and attributes could be simply modified by modifying this component.
 *
 * Attributes - should they be hard coded to this component
 * or they should be declared as separate class(es)
 * and component would only hold array of them.
 *
 * Should effect compoentn be merged with Attributes ?
 * those component are pretty interconnected anyway...
 * Effects in the essence, modify attributes
 */

USTRUCT()
struct FAttributeEffectEvent
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	AActor* ActorHit;

	UPROPERTY()
		TSubclassOf<class URPGEffectBase> EffectClass;
};

USTRUCT()
struct FAttributeRadialEffectEvent
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TArray<AActor*> ActorHits;

	UPROPERTY()
	TSubclassOf<class URPGEffectBase> EffectClass;
};

UCLASS(meta = (BlueprintSpawnableComponent), hidecategories = (Object, LOD, Lighting, Transform, Sockets, TextureStreaming))
class URPGAttributeComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) OVERRIDE;
	virtual void InitializeComponent() OVERRIDE;
	virtual void OnRegister() OVERRIDE;

public:
	virtual void TakeDamage(float DamageAmount, FName AttributeName, AActor* CausedBy);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = AttributeEvents)
	void OnDamageAppiled();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = AttributeEvents)
	void OnActorDeath();

public:

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Attributes")
		FOnRecievedEffect OnRecivedEffect;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Attributes")
		FOnAttributeChange OnAttributeChange;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Attributes")
		FOnEffectRemoved OnEffectRemoved;

	UFUNCTION(BlueprintNativeEvent, Category = "Attributes")
		void OnEffectAppiledToMe();

	UPROPERTY(BlueprintReadWrite, Category = "Effects")
		TArray<class URPGEffectPeriodic*> PeriodicEffectsList;
	FORCEINLINE TArray<class URPGEffectPeriodic*> GetEffects() { return PeriodicEffectsList; };
public:
	void RemoveEffect(class URPGEffectPeriodic* effectToRemove);

	/*
		Periodic Effects
	*/
	virtual void ApplyPeriodicEffect(class URPGEffectPeriodic* PeriodicEffect);

	/*
		Instant Effects
	*/
public:
	virtual void ApplyInstantEffects(TArray<FEffectInstant> EffectSpec, class URPGEffectPeriodic* ParentEffect);

	virtual void ApplyInstantEffect(FEffectInstant InstantEffectSpec);

private:
	bool CheckParentEffect(class URPGEffectPeriodic* ParentEffect);

public:
	UProperty* GetAttribute(FName AtributeName, TSubclassOf<URPGAttributeComponent> AttributeClass);

	UFUNCTION(BlueprintPure, Category = "Attributes")
		float GetNumericValue(FName AttributeName);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		void SetNumericValue(float value, FName AttributeName);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		bool IsSmaller(FName AttributeName, float SmallerThan);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		bool IsEqual(FName AttributeName, float EqualValue);

	/*
		Is A > B
	*/
	UFUNCTION(BlueprintCallable, Category = "Attributes")
		bool Compare(FName AttributeA, FName AttributeB);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		void ChangeAttribute(FName AttributeName, float ModValue, TEnumAsByte<EAttributeOperation> OperationType);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		void ModifyAttribute(FModdableAttributes AttributeMod, TEnumAsByte<EAttributeOperation> OperationType);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		void ModifyAttributeList(TArray<FModdableAttributes> Attributes, TEnumAsByte<EAttributeOperation> OperationType);

	/*
		Doesn't modify attribute. Just find it, add Value and return result for further use.
	*/
	UFUNCTION(BlueprintCallable, Category = "Attributes")
		float AttributeOperation(FName AttributeName, float Value, TEnumAsByte<EAttributeOperation> OperationType);

	/*
		Get list of attributes along with their values
	*/
	UFUNCTION(BlueprintCallable, Category = "Attributes")
		TArray<FModdableAttributes> GetAttributeList();

protected:
	//void DestroyEffect(class URPGEffectBase* EffectToDestroy);
	void SetPeriodicEffect(class URPGEffectPeriodic* newEffect);

private:
	UProperty* cachedProperty;
	//void GetOrCreateAttribute();
};
