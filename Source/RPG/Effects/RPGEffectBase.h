// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Object.h"
#include "Tickable.h"
#include "../RPGCharacter.h"
#include "../Common/RPGSharedEnums.h"
#include "RPGEffectBase.generated.h"
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEffectAppiledToTarget);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEffectTargetHealthChange, float, health);
enum EffectAlligment
{
	Effect_Negative,
	Effect_Neutral,
	Effect_Positive
};


/**
 * Effect is special presietant object. It cannot exist outside of actor, to which it is appiled. It would be
 pointless, since basic Effect functionality is to modify actor properties.
 Effects can last finite amount of time (or could be infinite), and are managed by EffectManagerComponent. Acotrs without this componenet,
 cannot be affected by Effects.

 Effects can also tick, every X amount of time, which allow to create damge over time abilities, or other abilities, that 
 do something at set interval of time.
 Effects are usualy appiled by Powers objects. But in theory can be appiled by anything that is able to supply target information.
 
 Function of this class, is to provide basic building blocks for defining effects and encaspulte common tasks (and more complex onces)
 into simple to use use functions. 
 With that main body of effect should be difined within blueprint. 
 1. Blueprints are easier to create by non-programmers.
 2. They compile fast.
 3. Overhead is neglible for provided benefits.
 4. They provide, most of the needed functionality. As currently entire system is based upon "simply" mmodyfing values stored in actors (see Common/RPG* header files for structs
 which provide most of the things which can be modified and which define our character).
 */
UCLASS(Blueprintable, BlueprintType)
class URPGEffectBase : public UObject , public FTickableGameObject
{
	GENERATED_UCLASS_BODY()

	virtual void Tick(float DeltaTime)  OVERRIDE;
	virtual bool IsTickable() const OVERRIDE;
	virtual TStatId GetStatId() const OVERRIDE;
public:

	//void Tick( float DeltaTime) OVERRIDE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=BaseProperties)
	float Duration;

	UPROPERTY()
	float TotalDuration;
	/**
	if true appiling next effect of the same class will refresh existing one, resetting it's duration
	and appiling new
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=BaseProperties)
	bool Refreshable;

	/**
	if true appiling effect the same class will add duration to the existing one. 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=BaseProperties)
	bool DurationStackable;

	/**
	if true appiling effect of the same class will accumulate effect, effect 
	i.e If base effect cause slow by 5%, adding next one will bring it to 10%, and then 15% etc.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=BaseProperties)
	bool EffectStackbale;

	/**
	Should effect tick. For example Damge Over Time effects will tick and deal damage on every tick.
	It also can be used to gradually subtract or add other values, like walking speed.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=BaseProperties)
	bool ShouldEffectTick;

	/**
	Set how often effect should tick. Every X seconds.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=BaseProperties)
	float TickDuration;
	/*
	Type of this effect. Do not forget to set it!
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=BaseProperties)
	TEnumAsByte<EEffectType> EffectType;
	/**
	Friendly name used to display info for player.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=BaseProperties)
	FName FriendlyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=BaseProperties)
	UParticleSystem* ParticleFX; //it's just for testing so we can attach something and see if it disappears. Well it might not be just for testing.

	/**
	Target affected by appiled effect
	*/
	UPROPERTY(BlueprintReadWrite, Category=BaseProperties)
	ARPGCharacter* AffectedTarget; //maybe change it to RPGCharacter ?

	/**
	Who appiled effect to target.
	*/
	UPROPERTY(BlueprintReadWrite, Category=BaseProperties)
	ARPGCharacter* CausedBy;

	UPROPERTY()
	bool IsEffectAppiled;
	
	UPROPERTY()
	bool IsEffectActive;

	/*
	* implementation specific to effect
	*/
	virtual void AddEffect();
	/** 
	Initialize timers for effect:
	Timer for calling remove
	Timer for calling finish event, to allow end clean up.
	*/
	virtual void Initialize();

	/** 
		Deinitialaize effect, effecticly setting it all ticking properties to 0 or false
	*/
	virtual void Deinitialize();

	//UPROPERTY(BlueprintAssignable)
	//FEffectAppiledToTarget OnEffectAppiledtoTarget;

	UFUNCTION(BlueprintNativeEvent, Category=PowerEffectEvents)
	void OnEffectAppiled();

	UFUNCTION(BlueprintImplementableEvent, Category=PowerEffectEvents)
	void OnEffectEnd();

	UFUNCTION(BlueprintImplementableEvent, Category=PowerEffectEvents)
	void OnEffectTick();

	UFUNCTION(BlueprintImplementableEvent, Category=PowerEffectEvents)
	void OnValueChanged();
	
	//UPROPERTY(BlueprintAssignable)
	//FEffectTargetHealthChange OnHealthChange;

	/**
		Gets current health from target, to which effect has been appiled.
		Dunno if that is efficient solution!
		In any case this should be struct FCharacterAttributes!
		And Second event for FCharacterDerivedAttributes!
	*/

	UFUNCTION(BlueprintNativeEvent, Category = PowerEffectEvents)
	void OnEffectTickFrame();

	/*
		Remove single effect from array (first match, first remove). Best used with OnEffectTick event, 
		so it will remove one effect every tick for said duration.
		Another appilance is, to remove effect every tick, count effects removed, and then at 
		this effect duration do something. Like Effects Removed x 5 = damage. appiled to target.
	*/
	UFUNCTION(BlueprintCallable, Category=PowerEffects)
	void RemoveSingleEffect(TEnumAsByte<EEffectType> appiledEffectType);

	UFUNCTION(BlueprintCallable, Category = EffectHelpers)
		void DecreaseAttributeForTime(float DecreaseValue);

	UFUNCTION(BlueprintCallable, Category = EffectHelpers)
		void TemporarlyDrainHealth(float DrainAmount);
	UFUNCTION(BlueprintCallable, Category = EffectHelpers)
		void TemporarlyDrainMaxHealth(float DrainAmount);

	UFUNCTION(BlueprintCallable, Category = EffectHelpers)
		uint8 GetHexesFromTarget();
protected:
	UPROPERTY(BlueprintReadOnly, Category=NativeProperties)
	int32 EffectsRemoved; //effects removed by this effect used with RemoveSingleEffect. If only used once, usually will be 0 or 1, if RemoveSingleEffect with ticker, it might vary!


		/*
	These are helper properties. It is possible to implement this in blueprint but:
	1. Each blueprint would have to implement them on their own.
	2. It would additional unneeded complexity.
	3. Native implementation should be at least a bit faster.

	These helpers get informations of target in every tick. So it is easier to track changes to attributes
	and implement effects that rely on current attribute state to do.. whatever they are doing.
	*/
	UPROPERTY(BlueprintReadOnly, Category=NativeHelpers)
	float TargetHealth;
	UPROPERTY(BlueprintReadOnly, Category=NativeHelpers)
	float TargetHealthPrecentage;
	UPROPERTY(BlueprintReadOnly, Category=NativeHelpers)
	float TargetEnergy;
	UPROPERTY(BlueprintReadOnly, Category=NativeHelpers)
	float TargetEndurance;

	UPROPERTY(BlueprintReadOnly, Category=NativeHelpers)
	float CauserHealth;
	UPROPERTY(BlueprintReadOnly, Category=NativeHelpers)
	float CauserEnergy;
	UPROPERTY(BlueprintReadOnly, Category=NativeHelpers)
	float CauserEndurance;

	void SetAttributeUpdates();

	/*
	this function will return truem when threshold (in precentage)
	will be reached, otherwise it will return false;
	*/
	UFUNCTION(BlueprintCallable, Category=EffectHelpers)
	bool CheckHealthTreshold(float threshold);

protected:
	virtual void SelfRemoveEffect();
private:
	float currentTickTime;
	float currentDuration;
	float OriginalConstitution;
	float AttributeDecrease;
	float MaxHealthDrain;
	void RestoreOriginalAttributes(float DecreasedValue);
	void RestoreOriginalHealth(float HealthValue);
	void RestoreOriginalMaxHealth(float HealthValue);
protected:
	virtual class UWorld* GetWorld() const OVERRIDE;
};
