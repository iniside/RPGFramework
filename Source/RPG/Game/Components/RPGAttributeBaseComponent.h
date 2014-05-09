// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "RPG.h"
#include "../../RPGSystem/Components/RPGAttributeComponent.h"
#include "RPGAttributeBaseComponent.generated.h"
/*
	In derived class we declare all needed attributes.
	It would be good idea to group them into Structs later.
	It could also be done as seprate object, although this approach is less blueprint friendly.
*/
UCLASS(meta = (BlueprintSpawnableComponent), hidecategories = (Object, LOD, Lighting, Transform, Sockets, TextureStreaming))
class URPGAttributeBaseComponent : public URPGAttributeComponent
{
	GENERATED_UCLASS_BODY()

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) OVERRIDE;
	virtual void InitializeComponent() OVERRIDE;
	virtual void OnRegister() OVERRIDE;
	virtual void PostInitProperties() OVERRIDE;
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Attributes")
		float Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Derived Attributes")
		float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Attributes")
		float HealthBase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Attributes")
		float HealthPerAttribute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Attributes")
		float Energy;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Derived Attributes")
		float MaxEnergy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Attributes")
		float EnergyBase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Attributes")
		float EnergyPerAttribute;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Attributes")
		float Stamina;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Derived Attributes")
		float MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Attributes")
		float StaminaBase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Attributes")
		float StaminaPerAttribute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes")
		float Constitution;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base Attributes")
		float ConstitutionMod;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes")
		float Strenght;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base Attributes")
		float StrenghtMod;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes")
		float Thoughness;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base Attributes")
		float ThougnessMod;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes")
		float Endurance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base Attributes")
		float EnduranceMod;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes")
		float Intelligence;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base Attributes")
		float IntelligenceMod;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes")
		float WillPower;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base Attributes")
		float WillPowerMod;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes")
		float Magic;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base Attributes")
		float MagicMod;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Derived Attributes")
		float SpellCritChance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Derived Attributes")
		float SpellCritMultiply;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Derived Attributes")
		float CritChance;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Derived Attributes")
		float CritMultiply;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Regeneration")
		bool CanRegenerateHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Regeneration")
		float HealthRegenInterval;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Regeneration")
		float HealthRegenValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Regeneration")
		bool CanRegenerateEnergy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Regeneration")
		float EnergyRegenInterval;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Regeneration")
		float EnergyRegenValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Regeneration")
		bool CanRegenerateStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Regeneration")
		float StaminaRegenInterval;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Regeneration")
		float StaminaRegenValue;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Weight")
		float EquipmentWeight;

	float SpellCastingSpeed;
	float SpellBaseCost;
	float AbilityWeightCostMod;

private:
	float currentHealthRegenInterval;
	float currentEnergyRegenInterval;
	float currentStaminaRegenInterval;
	float value;
	class URPGAttributesBase* GetOrCreateAttributes();
public:

	FORCEINLINE float GetHealth() { return Health; };
	FORCEINLINE void SetHealth(float value) { Health = value; };
	FORCEINLINE void SubtractHealth(float value) { Health -= value; };
	FORCEINLINE float GetMaxHealth() { return MaxHealth; };
	FORCEINLINE void SetMaxHealth() { MaxHealth = HealthBase + (Constitution * HealthPerAttribute); };

	FORCEINLINE float GetEnergy() { return Energy; };
	FORCEINLINE void SetEnergy(float value) { Energy = value; };
	FORCEINLINE void SubtractEnergy(float value) { Energy -= value; };
	FORCEINLINE float GetMaxEnergy() { return MaxEnergy; };
	FORCEINLINE void SetMaxEnergy() { MaxEnergy = EnergyBase + (IntelligenceMod * EnergyPerAttribute); };

	FORCEINLINE float GetStamina() { return Stamina; };
	FORCEINLINE void SubtractStamina(float value) { Stamina -= value; };
	FORCEINLINE float GetMaxStamina() { return MaxStamina; };
	FORCEINLINE void SetMaxStamina() { MaxStamina = StaminaBase + (EnduranceMod * StaminaPerAttribute); };


	FORCEINLINE float GetConstitution() { return Constitution; };
	FORCEINLINE void SetConstitution(float value) { Constitution = value; };
	FORCEINLINE void SubtractConstitution(float value) { Constitution -= value; };
	FORCEINLINE void AddConstitution(float value) { Constitution += value; };
	FORCEINLINE void SetConsitutionMod() { ConstitutionMod = (Constitution - 10) / 2; };
	FORCEINLINE float GetConsitutionMod() { return ConstitutionMod; };

	FORCEINLINE void SetIntelligenceMod(){ IntelligenceMod = (Intelligence - 10) / 2; };
	FORCEINLINE float GetIntelligenceMod(){ return IntelligenceMod; };

	FORCEINLINE void SetEnduranceMod(){ EnduranceMod = (Endurance - 10) / 2; };
	FORCEINLINE float GetEnduranceMod(){ return EnduranceMod; };

	FORCEINLINE bool SpellCritted() {
		float rngNum = FMath::FRandRange(1, 100);
		/*
		*/
		if (SpellCritChance >= rngNum)
			return true;
		return false;
	}
	//this is dumb implementation we need to account for other attributes like SpellCritChance as well ;)
	FORCEINLINE void SetSpellCritChance() { SpellCritChance = IntelligenceMod * 2; };
};

/*
this is pseudocode for calculating spellDamage;
DamageType_Spell(AttributeComponent* attribute, Blueprint)
{
	//order in which calculations are made is crucial!

	int32 finalDamage = Blueprint * attribute->SpellDamage;
	if(rand() < attribute->CritChange)
	{
		finalDamage *= CritMultiply;
	}
	
	//SpellProtection:
	finalDamage = finalDamage - (finalDamage * SpellProtection(0-1));
	
	//armorProtection
	finalDamage = finalDamage - armor;

	attribute->Health -= finalDamage;
}
*/