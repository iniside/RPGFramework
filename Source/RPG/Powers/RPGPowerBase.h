// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Object.h"
#include "../Common/RPGSharedEnums.h"
#include "../Items/RPGWeaponBase.h"
#include "RPGPowerBase.generated.h"

/**
 * This is base class for all powers in game.
 * Power is an action that can be performed by pawn, if said pawn have PowerManagerComponent.
 * like casting spell, throwing granade, shooting weapon, swinging sword. 
 Well, maybe those be better implemented in RPGWeapon ;)
 * but any special action like whirlwind sword, is essentialy power that trigger special animation, 
 have set duration (how long will character whirlwind ?), cost (how much endurance?)
 * cooldown (how long before I can use it again) and other costs defined.
 * power can also require character to have special weapon equiped, in order to use it.
 * powers are never placed directly in level. The class is data holder as well as contains 
 common function needed to perform power.
 * power logic/behavior should be defined withing blueprint, which will derive 
 from appropriate Power class.
 */
/* TODO
1. Add cast time handling (time between button press by player and triggering action). - semi done
2. Add resource cost handling. - done
3. Add weapon requierment. - semi done
4. Add attribute requirment.
5. Add cooldown handling (time after which power can be used again). - done
6. Add projectile shooting.
7. Add types of power, by enum.
8. Add mele type functions. Or create another power down the line in class hierarchy.
9. 
*/

DECLARE_DELEGATE_RetVal(float, FCastTimeDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCastTimeDelegateTwo, float, currentCastTime);

UENUM()
enum EPowerFireMode
{
	Channeled, //hold button to channel power.
	Channeled_Auto, //press button and power will channel automatically
	Instant, //pres button and fire instantly (after animation finish)
	CastTime, //press button and start casting
	CastTime_Charged //hold button begin cast. You must hold button for entire duration, otherwise
	//ability is canceled, and is not caseted
};

enum EPowerPreparationState
{
	Prepared,
	Unprepared
};

struct FPowerState
{
	bool IsCasting;
	bool IsCasted;
	bool IsOnCooldown;
	bool IsChanneled;
};
UCLASS(BlueprintType, Blueprintable)
class URPGPowerBase : public UObject, public FTickableGameObject
{
	GENERATED_UCLASS_BODY()

	/*
	We will need ticker for things like:
	1. Tracking cast time.
	2. Tracking cooldowns.
	It probably could be implemented in central place like PowerManagerCompoent (or specialized version EquipedPowerManager)
	But it would create unnesseary deelay as would need to iterate trough every power in loop, to set events and track their individual 
	timers.
	Instead we will make sure that each power is self-contained and can take care of it self.
	*/
	virtual void Tick(float DeltaTime)  OVERRIDE;
	virtual bool IsTickable() const OVERRIDE;
	virtual TStatId GetStatId() const OVERRIDE;


	bool IsPrepared;
	/*
	* if true first use, will make power prepared ie. aoe power will show targeting circle
	* second buttton press will activate power.
	*/
	UPROPERTY(EditAnywhere, Category=BaseProperties)
	bool NeedsPreparation;
	/**
	Resource Cost BEGIN
	**/
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Cost)
	float HealthCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Cost)
	float EnergyCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Cost)
	float EnduranceCost;
	/*
	Exhaustion is special cost. When power have it defined, it will "exhaust" resource for period of time, preventing it regeneration, beyond exhaustion threshold.
	for example character 200 energy. Power cost 40 energy + 20 exhaustion = total cost is 60. After using character is left with 140 energy. But can regenerate only 180.
	after exhaustion effect expires character can regen to 200.
	*/
	/*
	It actually should be moved into effect class. I mean create blueprint effect that will cause exhaustion to happen. Although these properties can remain
	to feed the blueprint with data.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Cost)
	float HealthExhaustion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Cost)
	float EnergyExhaustion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Cost)
	float EnduranceExhaustion;
	/**
	Resource Cost END
	**/
	/**
	Base Properties BEGIN
	**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=BaseProperties)
	float RechargeTime;

	UPROPERTY(EditAnywhere, Category=BaseProperties)
	TEnumAsByte<EPowerType> PowerType;
	UPROPERTY(EditAnywhere, Category=BaseProperties)
	TEnumAsByte<EPowerFireMode> PowerFireMode;

	
	/*
	Weapon that character must be using, to use power. Not not equiped, it must be drawn!
	*/
	UPROPERTY(EditAnywhere, Category=BaseProperties)
	TEnumAsByte<EWeaponType> RequiredWeapon;

	UPROPERTY(BlueprintReadWrite, Category=NativeVars)
	class ARPGCharacter* PowerOwner; //use TWeakObjectPtr because Garbage Collector couldn't grasp deferencing it while aburptly closing PIE and using normal pointer (;.
	//UPROPERTY(BlueprintReadWrite, Category=NativeVars)
	TWeakObjectPtr<class ARPGWeaponBase> MainCasterWeapon;

	UFUNCTION(BlueprintImplementableEvent)
		void OnPowerPrepared();

	/*
		Particle Effects BEGIN
	*/
	/*
		I wonder... Wouldn't it be better
		to move this into single blueprint function
		and designer would just plug all needed paremters including particle system
		either directly or trough variables...
	*/


	UFUNCTION(BlueprintCallable, Category=PowerFXEffects)
		void SpawnTrailEffect(UParticleSystem* trailFX, float trailSpeed, FName trailSpeedParam, FHitResult target, FName SocketName);
	
	/*
		Particle Effects BEGIN
	*/

	/**
	Base Properties End
	**/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Comments)
	FString Description;

	/**
	Blueprint helpers BEGIN
	**/
	/**
	Return FHitResult from Weapon impact point. Used for ranged attacks. Use different one for mele!
	*/
	UFUNCTION(BlueprintCallable, Category=PowerHelpers)
		FHitResult GetHitResult(float range, FName StartSocket); //or maybe create it as property ?
	UFUNCTION(BlueprintCallable, Category=PowerHelpers)
	AActor* GetActorHit();

	/*
	This will spawn projectile
	*/
	UFUNCTION(BlueprintCallable, Category=PowerHelpers)
		void SpawnProjectile(TSubclassOf<class ARPGProjectile> Projectile, float ProjectileSpeed, FName SocketName);

	/**
	this will spawn projectile at set location (extracted from Location)
	in set radius. Initial impact point in radius is determined randomly,
	and projectile behaviour might change it further.
	*/
	UFUNCTION(BlueprintCallable, Category=PowerHelpers)
		void SpawnProjectileAtLocationRadius(TSubclassOf<class ARPGProjectile> Projectile, FHitResult Impact, float Radius, float StartHeight, int32 ProjectileNumber);


	UFUNCTION(BlueprintCallable, Category = PowerHelpers)
		void SpawnEffectActorAtLocation(TSubclassOf<class ARPGEffectActor> effectActor, FHitResult HitLocation, float Height);

	/**
	targetToRemoveEffect - actor from which we will remove effect
	appiledEffectType = type of effects to remove

	return number of removed effects
	*/
	UFUNCTION(BlueprintCallable, Category=CharacterPowers)
	int32 RemovesEffect(AActor* targetToRemoveEffect, TEnumAsByte<EEffectType> appiledEffectType);
	/**
	Blueprint helpers END
	**/

	UAnimSequence* animSequence;
	UAnimMontage* animMontage;

public:
	inline void SetPowerOner(ARPGCharacter* owner) {PowerOwner = owner;};
	inline void SetMainCasterWeapon(TWeakObjectPtr<class ARPGWeaponBase> weapon) { MainCasterWeapon = weapon;};
	inline FString GetDescription() const {return Description;};
protected:
	UWorld* GetCurrentWorld() const;

	FPowerState PowerState;
public:
	/**
	Events BEGIN
	**/
	/**
	Will trigger power from input component. 
	*/

	/*
	this is used to handle various power requriments. Like subtracting resources
	checking if right weapon is equiped, wait for cast time etc.
	It all requriments are meet it will trigger OnCastPower, which can be used in blueprint.
	*/
	virtual void OnCastStart();
	virtual void OnCastStop();


	UFUNCTION(BlueprintNativeEvent)
	void OnCastPower();
	/*
	Event called on every frame tick.
	Might be especially useful, for beam-channel graphical effects, where we need,
	to constatnl update it's paremeters.
	*/
	UFUNCTION(BlueprintNativeEvent)
	void OnPowerTick();
	/**
	Events END
	**/
private:
	FHitResult RangedPowerTrace(const FVector& StartTrace, const FVector& EndTrace) const;
	FVector GetCameraDamageStarLocation(const FVector& AimDir) const;
	FVector GetCameraAim() const;
	FVector GetMuzzleDirection();
protected:
	float currentRechargeTime;
	float currentCastTime;
	EPowerPreparationState preparationState;
	
	/*
		this function is not particularly correct. As it also should return other casting locations that are not weapon. 
		Power should specific if it needs weapon, and the this function should act accordingly.
		It is not important now, but, it's worth remembering!
	*/
	USkeletalMeshComponent* GetWeaponMesh();
	//TWeakObjectPtr<ARPGWeaponBase> GetWeapon();
	FVector GetCastingLocation(FName SocketName);

	virtual class UWorld* GetWorld() const OVERRIDE;

public:
	float GetRechargeTime();

	/*
	Last actor that was hit by our power. It's very bad, but I want to checkout something.
	*/
	UPROPERTY(BlueprintReadOnly, Category=HUDHelpers)
	AActor* CurrentPowerTarget;
	
	FCastTimeDelegate CasTimeDelegate;
	FCastTimeDelegateTwo CasTimeDelegateTwo;
	UFUNCTION(BlueprintCallable, Category=HUDFunctions)
	float GetCurrentCastTime();// {return currentCastTime;}
	inline float GetCurrentRechargeTime() { return currentRechargeTime; }
};
