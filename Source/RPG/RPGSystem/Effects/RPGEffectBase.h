// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Object.h"
#include "Tickable.h"
#include "GameplayTagContainer.h"
#include "RPGEffectBase.generated.h"

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
class URPGEffectBase : public UObject , public FTickableGameObject
{
	GENERATED_UCLASS_BODY()

	virtual void Tick(float DeltaTime)  OVERRIDE;
	virtual bool IsTickable() const OVERRIDE;
	virtual TStatId GetStatId() const OVERRIDE;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=BaseProperties)
	float Duration;

	UPROPERTY()
	float TotalDuration;

	UPROPERTY(EditAnywhere, Category = "Base Properties")
	bool ShouldEffectTick;

	UPROPERTY(EditAnywhere, Category = "Base Properties")
	float TickDuration;

	UPROPERTY(EditAnywhere, Category = "Base Properties")
	bool StackDuration;

	//Like healing
	UPROPERTY(EditAnywhere, Instanced, Category = "Attribute Modification")
		float PositiveMod;

	//like damage
	UPROPERTY(EditAnywhere, Instanced, Category = "Attribute Modification")
		float NegativeMod;

	UPROPERTY(EditAnywhere, Category = "Base Properties")
		TEnumAsByte<EApplicationType> ApplicationType;

	/*
		Effect with the same Owned tags, can be affected by this effect
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer OwnedTags;

	/*
		Effect that I try to affect must have these tags.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
		FGameplayTagContainer RequiredTags;

	/*
		Effect with those tags should be ignored by this effect
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer IgnoredTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
		UDataTable* EffectData;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
		TArray<FName> AttributesToModify;

	UPROPERTY(BlueprintReadOnly, Category="Attributes")
	class URPGAttributeComponent* TargetAttributes;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	class URPGAttributeComponent* CauserAttributes;

public:
	FORCEINLINE void SetTarget(AActor* target) { Target = target; };
	FORCEINLINE void SetCauser(AActor* causer) { CausedBy = causer; };
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

	//UPROPERTY(BlueprintAssignable)
	//FEffectAppiledToTarget OnEffectAppiledtoTarget;

	UFUNCTION(BlueprintImplementableEvent, Category = PowerEffectEvents)
	void OnEffectAppiled();

	UFUNCTION(BlueprintImplementableEvent, Category=PowerEffectEvents)
	void OnEffectEnd();

	UFUNCTION(BlueprintImplementableEvent, Category=PowerEffectEvents)
	void OnEffectPeriod();

	UFUNCTION(BlueprintImplementableEvent, Category=PowerEffectEvents)
	void OnValueChanged();

	UFUNCTION(BlueprintImplementableEvent, Category = PowerEffectEvents)
	void OnEffectTick();

	//UFUNCTION(BlueprintCallable, Category = "Effects")
	//	void ApplyEffectFromEffect(TSubclassOf<class URPGEffectBase> Effect);

protected:
	void SelfRemoveEffect();
private:
	float currentPeriodTime;
	float currentDuration;
	float OriginalConstitution;
	float AttributeDecrease;
	float MaxHealthDrain;
	//class URPGAttributeComponent* targetAttributeComponent;
protected:
	virtual class UWorld* GetWorld() const OVERRIDE;
};
