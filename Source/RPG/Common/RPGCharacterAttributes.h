#pragma once
//#include "RPG.h"
#include "RPGCharacterAttributes.generated.h"


USTRUCT(BlueprintType)
struct FCharacterAttributes
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CharacterAttributes)
	float Strenght;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CharacterAttributes)
	float Dexterity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CharacterAttributes)
	float Cunning;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CharacterAttributes)
	float Constitution;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CharacterAttributes)
	float Thoughness;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CharacterAttributes)
	float WillPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CharacterAttributes)
	float Magic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CharacterAttributes)
	float Intelligence;
	FCharacterAttributes()
	{
	}
};
USTRUCT(BlueprintType)
struct FCharacterSecondaryAttributes
{
	GENERATED_USTRUCT_BODY()

	/*
	Weight of current equiped items (total)
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterSecondaryAttributes)
	float Encumbrance;

	/*
	Basic movment speed.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterSecondaryAttributes)
	float MovmentSpeed;


};
USTRUCT(BlueprintType)
struct FCharacterDerivedAttributes
{
	GENERATED_USTRUCT_BODY()
	/* 
	Current health of character
	**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CharacterStructAttributes)
	float Health;

	/* 
	Maximum amount of health that character have. 
	That number should only be changed while there health is increased permamently by some means.
	**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CharacterStructAttributes)
	float MaxHealth;
	
	/*
	Current energy of character
	**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CharacterAttributes)
	float Energy;

	/*
	Maximum amount of henergy that character have. 
	That number should only be changed while there health is increased permamently by some means.
	**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CharacterAttributes)
	float MaxEnergy;


	/** Current endurance of character*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CharacterAttributes)
	float Endurance;

	/** 
	Maximum amount of endurance that character have. 
	That number should only be changed while there health is increased permamently by some means.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CharacterAttributes)
	float MaxEndurance;

	/**
	Current health regeneration.
	Positive - health regeneration
	Negative - Health degeneration
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CharacterAttributes)
	float HealthRegeneration;
	FCharacterDerivedAttributes()
	{
	}
};

//note none of these attributes should be set directly! All of them should be calculcated from base stats, item, effects, and other!
USTRUCT(BlueprintType)
struct FCharacterDefensiveAttributes
{
	GENERATED_USTRUCT_BODY()
	/* 
	Armor reduce physical damage
	**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CharacterStructAttributes)
	float Armor;

	/**
	Just one stat for now.
	Resistance is specialized stat, that will reduce damage incoming only of certain type
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CharacterStructAttributes)
	float Resistance;


	FCharacterDefensiveAttributes()
	{
	}
};

/*
	These are helper, base values. They are NEVER used on their own.
	They exist only help with calculations of other attributes!
*/
USTRUCT(BlueprintType)
struct FAttributesHelpers
{
	GENERATED_USTRUCT_BODY()
	/* 
	Armor reduce physical damage
	**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CharacterStructAttributes)
	float BaseHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CharacterStructAttributes)
	float BaseEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CharacterStructAttributes)
	float BaseEndurance;

	/*
	These tree are bit cryptic.
	That's because I haven't really decided which attributes to use, how many of them
	and how exactly will equations look.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CharacterStructAttributes)
	float HealthPerConstitution;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CharacterStructAttributes)
	float EnergyPerAttribute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CharacterStructAttributes)
	float EndurancePerAttribute;

	/*
		Regeneration attributes
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterAttributes)
		float HealthRegeneration;
	/*
	 How much seconds before health is added to pool.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterAttributes)
		float HealthRegenPeriod;
	/**
	Just one stat for now.
	Resistance is specialized stat, that will reduce damage incoming only of certain type
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CharacterStructAttributes)
	float Resistance;


	FAttributesHelpers()
	{
	}
};