// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameFramework/SpringArmComponent.h"
#include "RPGCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterCast);

UCLASS(config=Game)
class ARPGCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()
public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) OVERRIDE;
	//Item Slots
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterItems)
		TSubobjectPtr<class USkeletalMeshComponent> ChestMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterItems)
		TSubobjectPtr<class USkeletalMeshComponent> HeadMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterItems)
		TSubobjectPtr<class USkeletalMeshComponent> LegMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterItems)
		TSubobjectPtr<class USkeletalMeshComponent> FootMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterItems)
		TSubobjectPtr<class USkeletalMeshComponent> HandsMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterItems)
		TSubobjectPtr<class USkeletalMeshComponent> ShouldersMesh;

	//Manager Components
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		TSubobjectPtr<class USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		TSubobjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Componenets)
		TSubobjectPtr<class URPGEquipmentManagerComponent> EquipmentManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Componenets)
		TSubobjectPtr<class URPGAbilityManagerComponent> AbilityManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Componenets)
		TSubobjectPtr<class URPGAttributeBaseComponent> Attributes;
	//Action Buttons
public:
	UFUNCTION()
		void ActionButtonOne();
	UFUNCTION()
		void ActionButtonOneReleased();

	//UPROPERTY(BlueprintReadOnly, Transient, Category = ActionBar)
	//	URPGPowerBase* ActionButtonOneObj;

	UFUNCTION()
		void ActionButtonTwo();
	//UPROPERTY(Transient)
	//	URPGPowerBase* ActionButtonTwoObj;
	
	//events
public:
	UPROPERTY(BlueprintAssignable, Category = "RPGCharacter")
	FOnCharacterCast OnCharacterCast;

	//overrrides
public:
	virtual void PostInitProperties() OVERRIDE;
	virtual void PostLoad() OVERRIDE;
	virtual void Tick(float DeltaSeconds) OVERRIDE;
	virtual void OnConstruction(const FTransform& Transform) OVERRIDE;
	virtual void BeginPlay() OVERRIDE;
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

	void SetWalkingSpeed();
};

