// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "RPG.h"
#include "../Components/RPGAttributeBaseComponent.h"
#include "../Components/RPGEquipmentManagerComponent.h"
#include "../RPGPlayerController.h"
#include "../RPGCharacter.h"
#include "../Items/RPGWeaponBase.h"
#include "../Shared/RPGEnums.h"
#include "GameAbility.h"


UGameAbility::UGameAbility(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}

void UGameAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/*
	If there is no component on actor then we don't initialize
	because powers need attributes to work properly.
*/
void UGameAbility::Initialize(APawn* owner, AController* instigator)
{
	Super::Initialize(owner, instigator);
	if (AbilityOwner)
	{
		if (GetWorld())
		{
			if (OwnerAttributeComp.IsValid())
			{
				OwnerAttributes = Cast<URPGAttributeBaseComponent>(OwnerAttributeComp.Get());
			}
			EquipComp = AbilityOwner->FindComponentByClass<URPGEquipmentManagerComponent>();

			if (OwnerAttributes.IsValid() && EquipComp.IsValid())
			{
				IsAbilityInitialized = true;
				return;
			}
			IsAbilityInitialized = false;
		}
		IsAbilityInitialized = false;
	}
	IsAbilityInitialized = false;
}

void UGameAbility::InputPressed()
{
	bool haveRequiredWeapon = false;
	if (IsAbilityInitialized)
	{
		//if (!CalculateFinalCost(BaseEnergyCost, BaseHealthCost, BaseStaminaCost))
			//return;
		/*
			We assign weapon on button press, to make sure that when player changed weapon
			we can check if there is required weapon.
		*/
		MainHand = EquipComp->MainWeapon;
		OffHand = EquipComp->OffHandWeapon;

		if (MainHand)
		{
			if (MainHand->WeaponType == RequiredWeapon)
			{
				//we could add return here. If have one weapon
				//we don't need to check for second one.
				haveRequiredWeapon = true;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("You don't have needed weapon"));
				return;
			}
		}
		if (OffHand)
		{
			if (OffHand->WeaponType == RequiredWeapon)
			{
				haveRequiredWeapon = true;
			}
			else if (!haveRequiredWeapon) //we need at least one compatibile weapon.
			{
				UE_LOG(LogTemp, Warning, TEXT("You don't have needed weapon"));
				return;
			}
		}
		Super::InputPressed();
	}
}

void UGameAbility::InputReleased()
{
	Super::InputReleased();
	//if (AbilityCastType == ECastType::Channeled)
	//{
	//	OnAbilityStop();
	//	isChanneled = false;
	//}
}

bool UGameAbility::CalculateFinalCost(float energyCost, float healthCost, float staminaCost)
{
	float FinalEnergyCost = energyCost; //make some clever calculations based on various attributes;
	float FinalHealthCost = healthCost;
	float FinalStaminaCost = staminaCost;
	//now we check:
	//if we got this far, there is no chance that component is null.
	//if ((OwnerAttributes->GetEnergy() >= FinalEnergyCost) &&
	//	(OwnerAttributes->GetHealth() >= FinalHealthCost) &&
	//	(OwnerAttributes->GetStamina() >= FinalStaminaCost)
	//	)
	//{
	//	OwnerAttributes->SubtractEnergy(FinalEnergyCost);
	//	OwnerAttributes->SubtractHealth(FinalHealthCost);
	//	OwnerAttributes->SubtractStamina(FinalStaminaCost);
	//	return true;
	//}

	//if (OwnerAttributeComp->GetHealth() >= FinalHealthCost)
	//{
	//	OwnerAttributeComp->SubtractHealth(FinalHealthCost);
	//}

	//if (OwnerAttributeComp->GetStamina() >= FinalStaminaCost)
	//{
	//	OwnerAttributeComp->SubtractStamina(FinalStaminaCost);
	//}
	return false;
}

FHitResult UGameAbility::GetHitResult(float Range, FName StartSocket)
{
	const FVector Origin = GetCastingLocation(StartSocket);
	const FVector ShootDir = GetCameraAim();

	const FVector StartTrace = GetCameraDamageStarLocation(ShootDir);
	const FVector EndTrace = (StartTrace + ShootDir * Range);
	FHitResult Impact = RangedPowerTrace(StartTrace, EndTrace);
	
	DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Black, true, 10.0f, 0.0f, 1.0f);

	if (Impact.GetActor())
	{
		float impactLen;
		FVector impactDir;
		const FVector AdjustedDir = (Impact.ImpactPoint - Origin).SafeNormal();
		Impact.ImpactPoint.ToDirectionAndLength(impactDir, impactLen);
		FHitResult hitResult = RangedPowerTrace(Origin, Impact.ImpactPoint); //Origin + impactDir*range);
		DrawDebugLine(GetWorld(), Origin, Impact.ImpactPoint, FColor::Blue, true, 10.0f, 0.0f, 1.0f);

		TargetAttributes = Impact.Actor->FindComponentByClass<URPGAttributeComponent>();

		if (hitResult.GetActor())
		{
			DrawDebugLine(GetWorld(), Origin, Impact.ImpactPoint, FColor::Red, true, 10.0f, 0.0f, 1.0f);
			return hitResult;
		}
		return Impact;
	}
	return Impact;
}

AActor* UGameAbility::GetActorHit(float Range, FName StartSocket)
{
	FVector Origin = GetCastingLocation(StartSocket);
	const FVector ShootDir = GetCameraAim();

	FVector StartTrace = GetCameraDamageStarLocation(ShootDir);
	const FVector EndTrace = (StartTrace + ShootDir * Range);
	FHitResult Impact = RangedPowerTrace(StartTrace, EndTrace);

	DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Black, true, 10.0f, 0.0f, 1.0f);

	if (Impact.GetActor())
	{
		float impactLen;
		FVector impactDir;
		const FVector AdjustedDir = (Impact.ImpactPoint - Origin).SafeNormal();
		Impact.ImpactPoint.ToDirectionAndLength(impactDir, impactLen);
		FHitResult hitResult = RangedPowerTrace(Origin, Impact.ImpactPoint); //Origin + impactDir*range);
		DrawDebugLine(GetWorld(), Origin, Impact.ImpactPoint, FColor::Blue, true, 10.0f, 0.0f, 1.0f);

		SetTargetAttributes(Impact.GetActor());

		if (hitResult.GetActor())
		{
			DrawDebugLine(GetWorld(), Origin, Impact.ImpactPoint, FColor::Red, true, 10.0f, 0.0f, 1.0f);
			return hitResult.GetActor();
		}
		return Impact.GetActor();
	}
	return nullptr;
}

void UGameAbility::SetTargetAttributes(AActor* targetActor)
{
	if (targetActor)
	{
		TArray<URPGAttributeBaseComponent*> attributeComps;
		targetActor->GetComponents<URPGAttributeBaseComponent>(attributeComps);
		for (URPGAttributeBaseComponent* attrComp : attributeComps)
		{
			TargetAttributes = attrComp;
			return;
		}
	}
}

void UGameAbility::SpawnTrailEffect(UParticleSystem* trailFX, float trailSpeed, FName trailSpeedParam, FHitResult target, FName SocketName)
{
	FVector Origin = GetCastingLocation(SocketName);
	if (target.GetActor())
	{
		if (trailFX)
		{
			UParticleSystemComponent* TrailPSC = UGameplayStatics::SpawnEmitterAtLocation(AbilityOwner, trailFX, Origin);
			if (TrailPSC)
			{
				const FVector AdjustedDir = (target.ImpactPoint - Origin).SafeNormal();
				FVector ParticleSpeed = AdjustedDir * trailSpeed;
				TrailPSC->SetVectorParameter(trailSpeedParam, ParticleSpeed);
			}
		}
	}
}


FVector UGameAbility::GetCameraDamageStarLocation(const FVector& AimDir) const
{
	FVector OutStartTrace = FVector::ZeroVector;
	if (Instigator)
	{
		FRotator UnusedRot;
		Instigator->GetPlayerViewPoint(OutStartTrace, UnusedRot);
		OutStartTrace = OutStartTrace + AimDir * ((AbilityOwner->GetActorLocation() - OutStartTrace) | AimDir);
		//OutStartTrace = OutStartTrace + AimDir * ((Instigator->GetActorLocation() - OutStartTrace));
	}
	return OutStartTrace;
}

FVector UGameAbility::GetCameraAim() const
{
	FVector FinalAim = FVector::ZeroVector;

	if (Instigator)
	{
		FVector CamLoc;
		FRotator CamRot;
		Instigator->GetPlayerViewPoint(CamLoc, CamRot);
		FinalAim = CamRot.Vector();
	}
	else if (AbilityOwner)
	{
		FinalAim = AbilityOwner->GetBaseAimRotation().Vector();
	}
	return FinalAim;
}

USkeletalMeshComponent* UGameAbility::GetWeaponMesh() const
{
	if (AbilityOwner)
	{

		/*
		We check both main hand and offhand for weapon. As both can be used
		to use any power.
		*/
		if (MainHand)
		{
			return MainHand->WeaponMesh;
		}
		else if (OffHand)
		{
			return OffHand->WeaponMesh;
		}
		return nullptr;
	}
	return NULL;
}

FVector UGameAbility::GetCastingLocation(FName SocketName) const
{
	const USkeletalMeshComponent* weapon = GetWeaponMesh();
	if (weapon)
	{
		return weapon->GetSocketLocation(SocketName);
	}
	//if (MainCasterWeapon.IsValid())
	//{
	//	return MainCasterWeapon->WeaponMesh->GetSocketLocation(SocketName);
	//}
	return FVector(0, 0, 0);
}

FVector UGameAbility::GetMuzzleDirection()
{
	//USkeletalMeshComponent* UseMesh = GetWeaponMesh();
	return GetWeaponMesh()->GetSocketRotation("MuzzleFlashSocket").Vector();
}

FHitResult UGameAbility::RangedPowerTrace(const FVector& StartTrace, const FVector& EndTrace) const
{
	FHitResult Hit(ForceInit);

	static FName PowerTag = FName(TEXT("PowerTrace"));
	FCollisionQueryParams TraceParams(PowerTag, false, AbilityOwner);
	TraceParams.bTraceComplex = false;
	TraceParams.bTraceAsyncScene = false;
	TraceParams.bReturnPhysicalMaterial = true;

	bool traceResult = GetWorld()->LineTraceSingle(Hit, StartTrace, EndTrace, ECollisionChannel::ECC_WorldDynamic, TraceParams);

	return Hit;
}

void UGameAbility::GetTableRow(UDataTable* Data, FName RowID, FAttributeData& OutData)
{
	if (Data)
	{
		FAttributeData* tempRow = NULL;
		tempRow = Data->FindRow<FAttributeData>(RowID, "ContextString");
		if (tempRow)
		{
			OutData = *tempRow;
		}
		//return tempRow;
	}
}