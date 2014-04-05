// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameFramework/SpringArmComponent.h"
#include "Common/RPGCharacterAttributes.h"
#include "Common/RPGCharacterStructs.h"
//#include "Components/RPGEquipmentManagerComponent.h"
#include "Powers/RPGPowerBase.h"
#include "RPGCharacter.generated.h"
//class URPGEquipmentManagerComponent;
//class URPGPowerBase;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterCast);

UCLASS(config=Game)
class ARPGCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterAttributes)
		FCharacterAttributes BaseAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterAttributes)
		FEffectsOnCharacter EffectsOnCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterAttributes)
	FAttributesHelpers AttributeHelpers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CharacterAttributes)
	bool CanRegenerateHealth;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterAttributes)
		FCharacterDerivedAttributes DerivedAttributes;
private:
	float currentHealthTickTime;
	//Item Slots
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterItems)
		TSubobjectPtr<class USkeletalMeshComponent> ChestMesh;

	//Manager Components
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Componenets)
		TSubobjectPtr<class URPGEquipmentManagerComponent> EquipmentManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Componenets)
		TSubobjectPtr<class URPGPowerManagerComponent> PowerManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Componenets)
		TSubobjectPtr<class URPGEffectManagerComponent> EffectManager;
	//Action Buttons
public:
	UFUNCTION()
		void ActionButtonOne();
	UFUNCTION()
		void ActionButtonOneReleased();
	UPROPERTY(BlueprintReadOnly, Transient, Category = ActionBar)
		URPGPowerBase* ActionButtonOneObj;

	UFUNCTION()
		void ActionButtonTwo();
	UPROPERTY(Transient)
		URPGPowerBase* ActionButtonTwoObj;
	
	//events
public:
	FOnCharacterCast OnCharacterCast;

	//overrrides
public:
	virtual void PostInitProperties() OVERRIDE;
	virtual void PostLoad() OVERRIDE;
	virtual void Tick(float DeltaSeconds) OVERRIDE;
	virtual void OnConstruction(const FTransform& Transform) OVERRIDE;
protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	//void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	/*void LookUpAtRate(float Rate);*/

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);
	UPROPERTY(EditAnywhere, Category = CameraOffsets)
		FVector LowOffset;
	UPROPERTY(EditAnywhere, Category = CameraOffsets)
		FVector MidOffset;
	UPROPERTY(EditAnywhere, Category = CameraOffsets)
		FVector HighOffset;
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) OVERRIDE;
	virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) OVERRIDE;
	// End of APawn interface

public:
	void SetHealth(float value);
	float GetMaxHealth();

	float GetCurrentEnergy();
	float GetCurrentEndurance();
	float GetCurrentHealth();

	void SubtractCurrentEnergy(float amount);
	void SubtractCurrentEndurance(float amount);
	void SubtractCurrentHealth(float amount);
};

