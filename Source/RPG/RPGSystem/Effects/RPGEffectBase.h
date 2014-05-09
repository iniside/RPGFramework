// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Object.h"
#include "Tickable.h"
#include "RPGEffectBase.generated.h"

UENUM()
enum EApplicationType
{
	InstantApplication,
	Periodic,
	Infinite
};

UCLASS(Blueprintable, BlueprintType)
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

	UPROPERTY(EditAnywhere, Category = "Base Properties")
		TEnumAsByte<EApplicationType> ApplicationType;

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

public:
	FORCEINLINE void SetTarget(AActor* target) { Target = target; };
	FORCEINLINE void SetCauser(AActor* causer) { CausedBy = causer; };
	/*
	* implementation specific to effect
	*/
	virtual void AddEffect();
	/** 
	*/
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
	void OnEffectTick();

	UFUNCTION(BlueprintImplementableEvent, Category=PowerEffectEvents)
	void OnValueChanged();

	UFUNCTION(BlueprintImplementableEvent, Category = PowerEffectEvents)
	void OnEffectTickFrame();

	/*
		Remove single effect from array (first match, first remove). Best used with OnEffectTick event, 
		so it will remove one effect every tick for said duration.
		Another appilance is, to remove effect every tick, count effects removed, and then at 
		this effect duration do something. Like Effects Removed x 5 = damage. appiled to target.
	*/


protected:
	virtual void SelfRemoveEffect();
private:
	float currentTickTime;
	float currentDuration;
	float OriginalConstitution;
	float AttributeDecrease;
	float MaxHealthDrain;
protected:
	virtual class UWorld* GetWorld() const OVERRIDE;
};
