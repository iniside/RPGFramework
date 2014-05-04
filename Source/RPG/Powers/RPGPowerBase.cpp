// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "RPG.h"
#include "../Common/RPGSharedEnums.h"
//#include "../RPGWeaponBase.h"
#include "../RPGProjectile.h"
#include "../RPGCharacter.h"
#include "../HUD/RPGHUD.h"
#include "../RPGPlayerController.h"
#include "../Components/RPGEquipmentManagerComponent.h"
#include "../Components/RPGEffectManagerComponent.h"
#include "../Effects/RPGEffectBase.h"
#include "../Effects/EffectActors/RPGProjectileSpawner.h"
#include "RPGPowerBase.h"


URPGPowerBase::URPGPowerBase(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
		
	
}

void URPGPowerBase::Tick( float DeltaTime)
{
	OnPowerTick();
}

//this should not return true by defaul.
//there is no reason to tick power that are not equiped by character, and hence are not going to be used.
//so we should check if power is equiped.
//but for now I will leave it this way, since it's not high priority. Just remember to change it later!
bool URPGPowerBase::IsTickable() const
{
	if(PowerOwner)
	{
		if (GetWorld())
		{
			return true;
		}
		return false;
	}
	return false;
}
TStatId URPGPowerBase::GetStatId() const
{
	return this->GetStatID();
}

/**
Events Begin
**/

void URPGPowerBase::OnCastStart()
{

}
float URPGPowerBase::GetCurrentCastTime() {return currentCastTime;}
//void URPGPowerBase::CastPower()
//{
//	if(!PowerOwner)
//		return;
//
//	OnCastPower();
//	PowerState.IsCasted = true;
//	PowerState.IsCasting = false;
//	PowerState.IsOnCooldown = true;
//}

void URPGPowerBase::OnCastStop()
{
}

void URPGPowerBase::OnCastPower_Implementation()
{
	//this does nothing. For now (;.
}

void URPGPowerBase::OnPowerTick_Implementation()
{
}
/**
Events END
**/


/**
Blueprint helpers BEGIN
**/
FHitResult URPGPowerBase::GetHitResult(float range, FName StartSocket)
{
	ARPGPlayerController* MyPC = Cast<ARPGPlayerController>(PowerOwner->Controller);
	//ARPGHUD* const MyHUD = (MyPC) ? Cast<ARPGHUD>(MyPC->GetHUD()) : NULL;
	//if(!MyHUD)
	//	return FHitResult(ForceInit);

	//MyHUD->AimDistance = range;
	FVector Origin = GetCastingLocation(StartSocket);
	const FVector ShootDir = GetCameraAim();
	//const FVector ShootDir2 = MyHUD->Crosshair3DPos;

	FVector StartTrace = GetCameraDamageStarLocation(ShootDir);
	const FVector EndTrace =  (StartTrace + ShootDir * range);
	//const FVector EndTrace =  ShootDir2;
	FHitResult Impact = RangedPowerTrace(StartTrace, EndTrace);
	//this is purerly for debug. Since UBT doesn't like #if, just remember to delete this
		//once you don't need it. I really don't want additonal normal if, just so I can check 
		//and uncheck it in blueprint -;-.
	bool supported = true;
	DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Black, true, 10.0f, 0.0f, 1.0f);
	//DrawDebugSphere(GetWorld(), StartTrace, 400.0f, 32, FColor::Red, true, 10.0f, 1.0f);
	if (Impact.GetActor())
	{
		float impactLen;
		FVector impactDir;
		const FVector AdjustedDir = (Impact.ImpactPoint - Origin).SafeNormal();
		Impact.ImpactPoint.ToDirectionAndLength(impactDir, impactLen);
		FHitResult hitResult = RangedPowerTrace(Origin, Impact.ImpactPoint); //Origin + impactDir*range);
		DrawDebugLine(GetWorld(), Origin, Impact.ImpactPoint, FColor::Blue, true, 10.0f, 0.0f, 1.0f);
		CurrentPowerTarget = Impact.GetActor();
		if(hitResult.GetActor())
		{
			DrawDebugLine(GetWorld(), Origin, Impact.ImpactPoint, FColor::Red, true, 10.0f, 0.0f, 1.0f);
			return hitResult;
		}
		return Impact;
	}
	return Impact;
}

AActor* URPGPowerBase::GetActorHit()
{
	AActor* actor = NULL;

	//FVector Origin = GetCastingLocation();
	//const FVector ShootDir = GetCameraAim();
	//FVector StartTrace = GetCameraDamageStarLocation(ShootDir);

	//const FVector EndTrace = (StartTrace + ShootDir * 10000.0f);
	//FHitResult Impact = RangedPowerTrace(StartTrace, EndTrace);
	//
	////if(world)
	////{
	//	//this is purerly for debug. Since UBT doesn't like #if, just remember to delete this
	//	//once you don't need it. I really don't want additonal normal if, just so I can check 
	//	//and uncheck it in blueprint -;-.
	//	DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Black, true, 10.0f, 0.0f, 1.0f);
	//	if (Impact.GetActor())
	//	{
	//		FHitResult CorrectImpact = RangedPowerTrace(Origin, Impact.ImpactPoint);
	//		DrawDebugLine(GetWorld(), Origin, CorrectImpact.ImpactPoint, FColor::Red, true, 10.0f, 0.0f, 1.0f);
	//		actor = CorrectImpact.GetActor();
	//		return actor;
	//	}
		return actor;
	//}
}

void URPGPowerBase::SpawnTrailEffect(UParticleSystem* trailFX, float trailSpeed, FName trailSpeedParam, FHitResult target, FName SocketName)
{
	FVector Origin = GetCastingLocation(SocketName);
	if(target.GetActor())
	{
		if(trailFX)
		{
			UParticleSystemComponent* TrailPSC = UGameplayStatics::SpawnEmitterAtLocation(PowerOwner, trailFX, Origin);
			if(TrailPSC)
			{
				const FVector AdjustedDir = (target.ImpactPoint - Origin).SafeNormal();
				FVector ParticleSpeed = AdjustedDir * trailSpeed;
				TrailPSC->SetVectorParameter(trailSpeedParam, ParticleSpeed);
			}
		}
	}
}


FVector URPGPowerBase::GetCameraDamageStarLocation(const FVector& AimDir) const
{
	ARPGPlayerController* PC = Cast<ARPGPlayerController>(PowerOwner->Controller);
	FVector OutStartTrace = FVector::ZeroVector;

	if(PC)
	{
		FRotator UnusedRot;
		PC->GetPlayerViewPoint(OutStartTrace, UnusedRot);
		OutStartTrace = OutStartTrace + AimDir * ((PowerOwner->GetActorLocation() - OutStartTrace) | AimDir);
		//OutStartTrace = OutStartTrace + AimDir * ((Instigator->GetActorLocation() - OutStartTrace));
	}
	return OutStartTrace;
}

FVector URPGPowerBase::GetCameraAim() const
{
	ARPGPlayerController* PC = Cast<ARPGPlayerController>(PowerOwner->Controller);
	FVector FinalAim = FVector::ZeroVector;

	if(PC)
	{
		FVector CamLoc;
		FRotator CamRot;
		PC->GetPlayerViewPoint(CamLoc, CamRot);
		FinalAim = CamRot.Vector();
	}
	else if(PowerOwner)
	{
		FinalAim = PowerOwner->GetBaseAimRotation().Vector();
	}
	return FinalAim;
}

USkeletalMeshComponent* URPGPowerBase::GetWeaponMesh()
{
	if(PowerOwner)
	{
		//USkeletalMeshComponent* lolo = PowerOwner->EquipmentManager->MainWeapon->WeaponMesh;
		//PowerOwner->EquipmentManager->MainWeapon->WeaponMesh.Reset();
		/*
			We check both main hand and offhand for weapon. As both can be used
			to use any power. 
		*/
		if (PowerOwner->EquipmentManager->MainWeapon->WeaponMesh != NULL)
		{
			return PowerOwner->EquipmentManager->MainWeapon->WeaponMesh;
		}
		else if (PowerOwner->EquipmentManager->OffHandWeapon->WeaponMesh != NULL)
		{
			return PowerOwner->EquipmentManager->OffHandWeapon->WeaponMesh;
		}
		return nullptr;
	}
	return NULL;
}
//
//TWeakObjectPtr<ARPGWeaponBase> URPGPowerBase::GetWeapon()
//{
//	if(PowerOwner)
//	{
//		//USkeletalMeshComponent* lolo = PowerOwner->EquipmentManager->MainWeapon->WeaponMesh;
//		//PowerOwner->EquipmentManager->MainWeapon->WeaponMesh.Reset();
//		return PowerOwner->EquipmentManager->MainWeapon;
//	}
//	return NULL;
//}

FVector URPGPowerBase::GetCastingLocation(FName SocketName)
{
	if (GetWeaponMesh() != NULL)
	{
		return GetWeaponMesh()->GetSocketLocation(SocketName);
	}
	if(MainCasterWeapon.IsValid())
	{
		return MainCasterWeapon->WeaponMesh->GetSocketLocation(SocketName);
	}
	return FVector(0,0,0);
}

FVector URPGPowerBase::GetMuzzleDirection()
{
	//USkeletalMeshComponent* UseMesh = GetWeaponMesh();
	return GetWeaponMesh()->GetSocketRotation("MuzzleFlashSocket").Vector();
}

FHitResult URPGPowerBase::RangedPowerTrace(const FVector& StartTrace, const FVector& EndTrace) const
{	
	FHitResult Hit(ForceInit);

	static FName PowerTag = FName(TEXT("PowerTrace"));
	FCollisionQueryParams TraceParams(PowerTag, false, PowerOwner);
	TraceParams.bTraceComplex = false;
	TraceParams.bTraceAsyncScene = false;
	TraceParams.bReturnPhysicalMaterial = true;
	
	UWorld* world = GetCurrentWorld();
	//if(world)
	//{
	bool traceResult = GetWorld()->LineTraceSingle(Hit, StartTrace, EndTrace, ECollisionChannel::ECC_WorldDynamic, TraceParams);
	//}
	return Hit;
}

int32 URPGPowerBase::RemovesEffect(AActor* targetToRemoveEffect, TEnumAsByte<EEffectType> appiledEffectType)
{
	int32 effectCount = 0;
	ARPGCharacter* TargetPawn = Cast<ARPGCharacter>(targetToRemoveEffect);
	int32 arrayNum = TargetPawn->EffectManager->EffectsList.Num(); //just checkin remove it!
	if(TargetPawn)
	{
		//use iterator. Array will dynamically change, as elements are removed in this loop (!!).
		//They are also removed outside of loop, when they expire.
		//Iterator will take care for it, for us.
		for(auto it = TargetPawn->EffectManager->EffectsList.CreateIterator(); it; ++it)
		{
			URPGEffectBase* effect  = TargetPawn->EffectManager->EffectsList[it.GetIndex()];
			if(effect->EffectType == appiledEffectType)
			{
				TargetPawn->EffectManager->RemoveEffect(effect); 
				effectCount++;
			}
		}
		return effectCount;
	}
	return effectCount;
}

void URPGPowerBase::SpawnProjectile(TSubclassOf<class ARPGProjectile> Projectile, float ProjectileSpeed, FName SocketName)
{
	//FVector Origin = GetCastingLocation();
	FVector Origin = GetCastingLocation(SocketName);
	FVector ShootDir = GetCameraAim();

	const float ProjectileAdjustRange = 10000.0f;
	const FVector StartTrace = GetCameraDamageStarLocation(ShootDir);
	const FVector EndTrace = StartTrace + ShootDir * ProjectileAdjustRange;
	FHitResult Impact = RangedPowerTrace(StartTrace, EndTrace);
	
	// and adjust directions to hit that actor
	if (Impact.bBlockingHit)
	{
		const FVector AdjustedDir = (Impact.ImpactPoint - Origin).SafeNormal();
		bool bWeaponPenetration = false;

		const float DirectionDot = FVector::DotProduct(AdjustedDir, ShootDir);
		if (DirectionDot < 0.0f)
		{
			// shooting backwards = weapon is penetrating
			bWeaponPenetration = true;
		}
		else if (DirectionDot < 0.5f)
		{
			// check for weapon penetration if angle difference is big enough
			// raycast along weapon mesh to check if there's blocking hit

			FVector MuzzleStartTrace = Origin - GetMuzzleDirection() * 150.0f;
			FVector MuzzleEndTrace = Origin;
			FHitResult MuzzleImpact = RangedPowerTrace(MuzzleStartTrace, MuzzleEndTrace);

			if (MuzzleImpact.bBlockingHit)
			{
				bWeaponPenetration = true;
			}
		}

		if (bWeaponPenetration)
		{
			// spawn at crosshair position
			Origin = Impact.ImpactPoint - ShootDir * 10.0f;
		}
		else
		{
			// adjust direction to hit
			ShootDir = AdjustedDir;
		}
	}

	FTransform SpawnTM(ShootDir.Rotation(), Origin);
	ARPGProjectile* proj = Cast<ARPGProjectile>(UGameplayStatics::BeginSpawningActorFromClass(PowerOwner, Projectile, SpawnTM));
	if (proj)
	{
		proj->Instigator = PowerOwner;
		proj->SetOwner(PowerOwner);
		proj->InitVelocity(ShootDir);

		UGameplayStatics::FinishSpawningActor(proj, SpawnTM);
	}

}

void URPGPowerBase::SpawnProjectileAtLocationRadius(TSubclassOf<class ARPGProjectile> Projectile, FHitResult Impact, float Radius, float StartHeight, int32 ProjectileNumber)
{
	//if(Impact.GetActor())
	//{
	//	
	//	DrawDebugSphere(GetWorld(), Impact.ImpactPoint, Radius, 32, FColor::Yellow, false, 10.0f);
	//	for(int32 i = 0; i < ProjectileNumber; i++)
	//	{
	//		FVector ImpactVector = FVector::ZeroVector;
	//		FVector StartVector;
	//		/*Impact.ImpactPoint.Z = Impact.ImpactPoint.Z + 800.0f;
	//		Impact.ImpactPoint.X = FMath::Clamp(Impact.ImpactPoint.X + FMath::RandRange(-Radius, Radius), (Impact.ImpactPoint.X - Radius), (Impact.ImpactPoint.X + Radius)); 
	//		Impact.ImpactPoint.Y = FMath::Clamp(Impact.ImpactPoint.Y + FMath::RandRange(-Radius, Radius), (Impact.ImpactPoint.Y - Radius), (Impact.ImpactPoint.Y + Radius));*/
	//		
	//		StartVector.Z = Impact.ImpactPoint.Z+StartHeight;
	//		//StartVector.X = Impact.ImpactPoint.X;
	//		//StartVector.Y = Impact.ImpactPoint.Y;
	//		float x = FMath::RandRange((-1) * Radius, Radius);
	//		float y = FMath::RandRange((-1) * Radius, Radius);
	//		StartVector.X = FMath::Clamp(Impact.ImpactPoint.X + x, (Impact.ImpactPoint.X - Radius-50), (Impact.ImpactPoint.X + Radius+50)); 
	//		StartVector.Y = FMath::Clamp(Impact.ImpactPoint.Y + y, (Impact.ImpactPoint.Y - Radius-50), (Impact.ImpactPoint.Y + Radius+50));
	//		ImpactVector = Impact.ImpactPoint;
	//		ImpactVector.Z = Impact.ImpactPoint.Z + StartHeight;
	//		ImpactVector.X = FMath::Clamp(Impact.ImpactPoint.X + FMath::RandRange((-1) * Radius, Radius), (Impact.ImpactPoint.X - Radius), (Impact.ImpactPoint.X + Radius)); 
	//		ImpactVector.Y = FMath::Clamp(Impact.ImpactPoint.Y + FMath::RandRange((-1) * Radius, Radius), (Impact.ImpactPoint.Y - Radius), (Impact.ImpactPoint.Y + Radius));
	//		ImpactVector = ImpactVector + FVector(0,0,-3000);
	//		//FVector EndTrace = ImpactVector*1000;
	//		FVector AdjustedDir = (ImpactVector - StartVector).SafeNormal();
	//		DrawDebugLine(GetCurrentWorld(), StartVector, ImpactVector, FColor::Blue, false, 10.0f, 0.0f, 2.0f);
	//		//Impact.ImpactPoint.Y = Impact.ImpactPoint.Y + 500.0f;
	//		//Impact.ImpactPoint.X = Impact.ImpactPoint.X + 500.0f;
	//		FHitResult ProperHit = RangedPowerTrace(StartVector, ImpactVector);
	//		
	//		DrawDebugLine(GetCurrentWorld(), StartVector, ProperHit.ImpactPoint, FColor::Green, false, 10.0f, 0.0f, 2.0f);
	//		FTransform SpawnTM(FRotator(0,0,0), StartVector);
	//		//FTransform SpawnTM(ProperHit.ImpactPoint.Rotation(), StartVector);
	//		ARPGProjectile* proj = Cast<ARPGProjectile>(UGameplayStatics::BeginSpawningActorFromClass(PowerOwner, Projectile, SpawnTM));
	//		if (proj)
	//		{
	//			proj->Instigator = PowerOwner;
	//			proj->SetOwner(PowerOwner);
	//			//ProperHit.ImpactPoint;
	//			//ImpactVector.Z = ImpactVector.Z * (-1);
	//			FVector AdjustedDir = (ProperHit.ImpactPoint - StartVector).SafeNormal();
	//			ProperHit.ImpactNormal = ProperHit.ImpactNormal * (-1);
	//			
	//			proj->InitVelocity(ProperHit.ImpactNormal);
	//			//DrawDebugSphere(GetWorld(), StartVector, proj->CollisionSphereRadius, 32, FColor::Red, false, 10.0f);
	//			UGameplayStatics::FinishSpawningActor(proj, SpawnTM);
	//		}
	//	}
	//}
}
void URPGPowerBase::SpawnEffectActorAtLocation(TSubclassOf<class ARPGEffectActor> effectActor, FHitResult HitLocation, float Height)
{
	//ok this is dumb implementation just to check some things..
	
	HitLocation.ImpactPoint.Z += Height;

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.bNoCollisionFail = true;
	SpawnInfo.Owner = PowerOwner;
	SpawnInfo.Instigator = NULL;
	
	//ARPGProjectileSpawner* spawner = GetWorld()->SpawnActor<ARPGProjectileSpawner>(effectActor, HitLocation.ImpactNormal, FRotator(0, 0, 0), SpawnInfo);
	
	FTransform SpawnTM(FRotator(0, 0, 0), HitLocation.ImpactPoint);
	//spawner->OnConstruction(SpawnTM);
	//spawner->PostActorConstruction();
	ARPGProjectileSpawner* spawner = Cast<ARPGProjectileSpawner>(UGameplayStatics::BeginSpawningActorFromClass(PowerOwner, effectActor, SpawnTM));
	if (spawner)
	{
		spawner->SetSpawnLocation(HitLocation.ImpactPoint);
		UGameplayStatics::FinishSpawningActor(spawner, SpawnTM);
	}
}
/**
Blueprint helpers END
**/
float URPGPowerBase::GetRechargeTime()
{
	return currentRechargeTime;
}

UWorld* URPGPowerBase::GetWorld() const
{
	if (!PowerOwner)
		return NULL;
	UWorld* const World = GEngine->GetWorldFromContextObject(PowerOwner);
	return World ? World : nullptr;
}

UWorld* URPGPowerBase::GetCurrentWorld() const
{
	if(!PowerOwner)
		return NULL;
	UWorld* const World = GEngine->GetWorldFromContextObject(PowerOwner);
    return World ? World : nullptr;
}