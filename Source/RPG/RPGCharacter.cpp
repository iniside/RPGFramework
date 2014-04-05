// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "RPG.h"
#include "RPGPlayerController.h"
#include "Components/RPGEquipmentManagerComponent.h"
#include "Components/RPGPowerManagerComponent.h"
#include "Components/RPGEffectManagerComponent.h"
#include "RPGCharacter.h"

//////////////////////////////////////////////////////////////////////////
// ARPGCharacter

ARPGCharacter::ARPGCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// Set size for collision capsule
	CapsuleComponent->InitCapsuleSize(42.f, 96.0f);
	LowOffset = FVector(-380.0f, 35.0f, 15.0f);
	MidOffset = FVector(-380.0f, 35.0f, 60.0f);
	HighOffset = FVector(-380.0f, 35.0f, 150.0f); //x,y,z
	// set our turn rates for input
	//BaseTurnRate = 45.f;
	//BaseLookUpRate = 45.f;
	PrimaryActorTick.bCanEverTick = true;
	ChestMesh = PCIP.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("ChestMesh"));
	ChestMesh->AttachParent = Mesh;

	EquipmentManager = PCIP.CreateDefaultSubobject<URPGEquipmentManagerComponent>(this, TEXT("EquipmentManager"));
	
	PowerManager = PCIP.CreateDefaultSubobject<URPGPowerManagerComponent>(this, TEXT("PowerManager"));
	
	EffectManager = PCIP.CreateDefaultSubobject<URPGEffectManagerComponent>(this, TEXT("EffectManager"));
	EffectManager->Activate();
	EffectManager->bAllowConcurrentTick = true;
	// Configure character movement
	CharacterMovement->bOrientRotationToMovement = true; // Character moves in the direction of input...	

}
void ARPGCharacter::CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult)
{
	FVector Loc, Pos, HitLocation, HitNormal, EyeLoc, FinalPos;
	FRotator Rot, EyeRot;
	FPOV OrigPOV;
	//It should be header file, but for now it will work.
	//FVector LowOffset = FVector(-380.0f,35.0f, 15.0f);
	//FVector MidOffset = FVector(-380.0f,35.0f, 60.0f);
	//FVector HighOffset = FVector(-380.0f,35.0f,150.0f); //x,y,z

	//for some reason using blueprint Offset doesn't work.
	//so we redecalre it locally and assign value trough blueprint.
	FVector LowOffset = this->LowOffset;
	FVector MidOffset = this->MidOffset;
	FVector HighOffset = this->HighOffset; //x,y,z
	//APlayerController* PC;
	//int32 sizeX, sizeY;

	//if(OutVT.Target)
	//{
	//	PC = Cast<APlayerController>(OutVT.Target->GetInstigatorController());
	//	PC->GetViewportSize(sizeX, sizeY);
	//	float aspectRatio = 0.0f;
	//	aspectRatio = (float)sizeX / (float)sizeY;
	//	if(aspectRatio >= 1.333f && aspectRatio < 1.777f)
	//	{
	//		LowOffset = FVector(-100.0f,10.0f, 5.0f);
	//		MidOffset = FVector(-100.0f,10.0f, 0.0f);
	//		HighOffset = FVector(-110.0f,10.0f,80.0f); 
	//	}
	//	else if (aspectRatio > 1.76f)
	//	{
	//		LowOffset = FVector(-120.0f,10.0f, 5.0f);
	//		MidOffset = FVector(-200.0f,10.0f, 0.0f);
	//		HighOffset = FVector(-190.0f,10.0f,70.0f); 
	//	}
	//	else
	//	{
	//		LowOffset = FVector(-120.0f,10.0f, 5.0f);
	//		MidOffset = FVector(-200.0f,10.0f, 0.0f);
	//		HighOffset = FVector(-180.0f,10.0f,80.0f); 
	//	}
	//}


	FVector FinalOffset = MidOffset;

	float pitchXX = 0;
	float pitchZZ = 0;
	//OrigPOV = OutVT.POV;
	//OutVT.Target->GetActorEyesViewPoint(EyeLoc, EyeRot);
	this->GetActorEyesViewPoint(EyeLoc, EyeRot);
	Loc = this->GetActorLocation();
	//normalize axis to space between -180 and 180,
	// we need because by default rotator is between 0 and 360
	float normalizedEyeRotPitch = EyeRot.NormalizeAxis(EyeRot.Pitch);
	//we do everything in Absolute numbers and then just multipli by -/+1
	//looking up
	if (EyeRot.NormalizeAxis(EyeRot.Pitch) > 0.0f)
	{
		//FinalOffset = LowOffset;
		float offsetX = FMath::Abs(LowOffset.X) / LowOffset.X;
		float offsetZ = FMath::Abs(LowOffset.Z) / LowOffset.Z;

		//HighOffset.X = math.Abs(HighOffset.X);
		//HighOffset.Z = math.Abs(HighOffset.Z);
		LowOffset = LowOffset.GetAbs();
		MidOffset = MidOffset.GetAbs();
		FinalOffset = FinalOffset.GetAbs(); //we convert to absolute number everything is +

		float pitchX = (LowOffset.X - MidOffset.X) / 60; //-?
		pitchXX = pitchX * normalizedEyeRotPitch; // -
		pitchXX = FMath::Abs(pitchXX); // make sure result is +
		float pitchZ = (LowOffset.Z - MidOffset.Z) / 60; //180 / -60 //-
		pitchZZ = pitchZ * normalizedEyeRotPitch; // -
		pitchZZ = FMath::Abs(pitchZZ); //make sure it is +

		FinalOffset.X = FinalOffset.X + (pitchXX);
		FinalOffset.X = FMath::Clamp(FinalOffset.X, LowOffset.X, MidOffset.X) * offsetX;//we have issue here what if the values will be positive ?
		FinalOffset.Z = FinalOffset.Z + (pitchZZ);
		if (LowOffset.Z > MidOffset.Z)//guess other offset adjustments also would need that.
		{
			FinalOffset.Z = FMath::Clamp(FinalOffset.Z, MidOffset.Z, LowOffset.Z) * offsetZ;
		}
		else
		{
			FinalOffset.Z = FMath::Clamp(FinalOffset.Z, LowOffset.Z, MidOffset.Z) * offsetZ;
		}
		FinalOffset.Y = LowOffset.Y;
	} //looking down
	else if (EyeRot.NormalizeAxis(EyeRot.Pitch) < 0.0f)
	{
		float offsetX = FMath::Abs(HighOffset.X) / HighOffset.X;
		float offsetZ = FMath::Abs(HighOffset.Z) / HighOffset.Z;

		//HighOffset.X = math.Abs(HighOffset.X);
		//HighOffset.Z = math.Abs(HighOffset.Z);
		HighOffset = HighOffset.GetAbs();
		MidOffset = MidOffset.GetAbs();
		FinalOffset = FinalOffset.GetAbs(); //we convert to absolute number everything is +

		float pitchX = (HighOffset.X - MidOffset.X) / -60; //-?
		pitchXX = pitchX * normalizedEyeRotPitch; // -
		pitchXX = FMath::Abs(pitchXX); // make sure result is +
		float pitchZ = (HighOffset.Z - MidOffset.Z) / -60; //180 / -60 //-
		pitchZZ = pitchZ * normalizedEyeRotPitch; // -
		pitchZZ = FMath::Abs(pitchZZ);

		FinalOffset.X = FinalOffset.X + (pitchXX);
		FinalOffset.X = FMath::Clamp(FinalOffset.X, MidOffset.X, HighOffset.X) * offsetX;//we have issue here what if the values will be positive ?
		FinalOffset.Z = FinalOffset.Z + (pitchZZ);
		FinalOffset.Z = FMath::Clamp(FinalOffset.Z, MidOffset.Z, HighOffset.Z) * offsetZ;
		FinalOffset.Y = HighOffset.Y;
	} //looking directly straight pitch = 0, dunno if that ever happen.
	else
	{
		FinalOffset = MidOffset;
	}

	FinalPos = EyeLoc;
	//FinalOffset = FinalOffset.SafeNormal();
	FinalPos += FRotationMatrix(EyeRot).TransformVector(FinalOffset);
	//FinalPos += FRotationMatrix(EyeRot).TransformPosition(FinalOffset);
	OutResult.Location = FinalPos;
	OutResult.Rotation = EyeRot;
	OutResult.AspectRatio = 1.7777779;
	OutResult.bConstrainAspectRatio = true;
	/*OutVT.POV.Location = FinalPos;
	OutVT.POV.Rotation = EyeRot;*/
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARPGCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("ActionButtonOne", IE_Pressed, this, &ARPGCharacter::ActionButtonOne);
	InputComponent->BindAction("ActionButtonTwo", IE_Pressed, this, &ARPGCharacter::ActionButtonTwo);
	InputComponent->BindAxis("MoveForward", this, &ARPGCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ARPGCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	//InputComponent->BindAxis("TurnRate", this, &ARPGCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	//InputComponent->BindAxis("LookUpRate", this, &ARPGCharacter::LookUpAtRate);

	//// handle touch devices
	//InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ARPGCharacter::TouchStarted);
}

void ARPGCharacter::PostInitProperties()
{
	Super::PostInitProperties();
}
void ARPGCharacter::PostLoad()
{
	Super::PostLoad();
}
void ARPGCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	DerivedAttributes.MaxHealth = ((BaseAttributes.Constitution - 10) / 2) * AttributeHelpers.HealthPerConstitution + AttributeHelpers.BaseHealth;
	DerivedAttributes.Health = 0;
}
void ARPGCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (DerivedAttributes.Health <= DerivedAttributes.MaxHealth)
	{
		if (CanRegenerateHealth)
		{
			currentHealthTickTime += DeltaSeconds;
			if (currentHealthTickTime >= AttributeHelpers.HealthRegenPeriod)
			{
				DerivedAttributes.Health += AttributeHelpers.HealthRegeneration;
				currentHealthTickTime = 0;
			}
		}
	}
}
void ARPGCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

//void ARPGCharacter::TurnAtRate(float Rate)
//{
//	// calculate delta for this frame from the rate information
//	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
//}
//
//void ARPGCharacter::LookUpAtRate(float Rate)
//{
//	// calculate delta for this frame from the rate information
//	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
//}

void ARPGCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		FRotator Rotation = GetBaseAimRotation();

		FVector Location; //not used, just need for below.
		//Controller->GetPlayerViewPoint(Location, Rotation);
		Rotation.Normalize();
		FRotator YawRotation(0, Rotation.Yaw, 0);
		//const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis( EAxis::X );
		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ARPGCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		//const FRotator Rotation = Controller->GetPlayerViewPoint();
		FRotator Rotation;
		FVector Location; //not used, just need for below.
		Controller->GetPlayerViewPoint(Location, Rotation);
		Rotation.Normalize();
		FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		//const FVector Direction = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::Y);
		const FVector Direction = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ARPGCharacter::ActionButtonOne()
{
	ARPGPlayerController* PC = Cast<ARPGPlayerController>(Controller);
	if (PC)
	{
		ActionButtonOneObj->OnCastStart();
		OnCharacterCast.Broadcast();
	}
}

void ARPGCharacter::ActionButtonOneReleased()
{
	ARPGPlayerController* PC = Cast<ARPGPlayerController>(Controller);
	if (PC)
	{
		ActionButtonOneObj->OnCastStop();
	}
}

void ARPGCharacter::ActionButtonTwo()
{
	ARPGPlayerController* PC = Cast<ARPGPlayerController>(Controller);
	if (PC)
	{
		if (ActionButtonTwoObj != NULL)
		{
			ActionButtonTwoObj->OnCastStart();
			OnCharacterCast.Broadcast();
		}
	}
}

void ARPGCharacter::SetHealth(float value)
{
	DerivedAttributes.Health = value;
}
float ARPGCharacter::GetMaxHealth()
{
	DerivedAttributes.MaxHealth = ((BaseAttributes.Constitution - 10) / 2) * AttributeHelpers.HealthPerConstitution;
	return DerivedAttributes.MaxEnergy;
}
float ARPGCharacter::GetCurrentEnergy()
{
	return DerivedAttributes.Energy;
}
float ARPGCharacter::GetCurrentEndurance()
{
	return DerivedAttributes.Endurance;
}
float ARPGCharacter::GetCurrentHealth()
{
	return DerivedAttributes.Health;
}

void ARPGCharacter::SubtractCurrentEnergy(float amount)
{
	DerivedAttributes.Energy = DerivedAttributes.Energy - amount;
}
void ARPGCharacter::SubtractCurrentEndurance(float amount)
{
	DerivedAttributes.Endurance = DerivedAttributes.Endurance - amount;
}
void ARPGCharacter::SubtractCurrentHealth(float amount)
{
	DerivedAttributes.Health = DerivedAttributes.Health - amount;
}