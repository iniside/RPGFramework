// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "RPG.h"
#include "../../RPGProjectile.h"
#include "RPGProjectileSpawner.h"


ARPGProjectileSpawner::ARPGProjectileSpawner(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARPGProjectileSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARPGProjectileSpawner::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	OnSpawned();
}

void ARPGProjectileSpawner::SpawnProjectile()
{
	FVector currentLocation = GetActorLocation();
	DrawDebugSphere(GetWorld(), currentLocation, Radius, 32, FColor::Yellow, false, 10.0f);
	if (Projectile)
	{
		
		FVector ImpactVector = FVector::ZeroVector;
		FVector StartVector;

		//StartVector.Z = Impact.ImpactPoint.Z + StartHeight;
		//StartVector.X = Impact.ImpactPoint.X;
		//StartVector.Y = Impact.ImpactPoint.Y;
		float x = FMath::RandRange((-1) * Radius, Radius);
		float y = FMath::RandRange((-1) * Radius, Radius);
		currentLocation.X = FMath::Clamp(currentLocation.X + x, (currentLocation.X - Radius - 50), (currentLocation.X + Radius + 50));
		currentLocation.Y = FMath::Clamp(currentLocation.Y + y, (currentLocation.Y - Radius - 50), (currentLocation.Y + Radius + 50));
		//ImpactVector = Impact.ImpactPoint;
		//ImpactVector.Z = Impact.ImpactPoint.Z + StartHeight;
		//ImpactVector.X = FMath::Clamp(Impact.ImpactPoint.X + FMath::RandRange((-1) * Radius, Radius), (Impact.ImpactPoint.X - Radius), (Impact.ImpactPoint.X + Radius));
		//ImpactVector.Y = FMath::Clamp(Impact.ImpactPoint.Y + FMath::RandRange((-1) * Radius, Radius), (Impact.ImpactPoint.Y - Radius), (Impact.ImpactPoint.Y + Radius));
		//ImpactVector = ImpactVector + FVector(0, 0, -3000);
		//FVector EndTrace = ImpactVector*1000;
		FVector AdjustedDir = (ImpactVector - StartVector).SafeNormal();
		DrawDebugLine(GetWorld(), currentLocation, ImpactVector, FColor::Blue, false, 10.0f, 0.0f, 2.0f);
		//Impact.ImpactPoint.Y = Impact.ImpactPoint.Y + 500.0f;
		//Impact.ImpactPoint.X = Impact.ImpactPoint.X + 500.0f;
		//FHitResult ProperHit = RangedPowerTrace(StartVector, ImpactVector);

		//DrawDebugLine(GetWorld(), StartVector, ProperHit.ImpactPoint, FColor::Green, false, 10.0f, 0.0f, 2.0f);
		FTransform SpawnTM(FRotator(0, 0, 0), StartVector);
		//FTransform SpawnTM(ProperHit.ImpactPoint.Rotation(), StartVector);
		ARPGProjectile* proj = Cast<ARPGProjectile>(UGameplayStatics::BeginSpawningActorFromClass(this, Projectile, SpawnTM));
		if (proj)
		{
			//proj->Instigator = PowerOwner;
			//proj->SetOwner(PowerOwner);
			//ProperHit.ImpactPoint;
			//ImpactVector.Z = ImpactVector.Z * (-1);
			//FVector AdjustedDir = (ProperHit.ImpactPoint - StartVector).SafeNormal();
			//ProperHit.ImpactNormal = ProperHit.ImpactNormal * (-1);

			//proj->InitVelocity(ProperHit.ImpactNormal);
			//DrawDebugSphere(GetWorld(), StartVector, proj->CollisionSphereRadius, 32, FColor::Red, false, 10.0f);
			UGameplayStatics::FinishSpawningActor(proj, SpawnTM);
		}
	}
}

void ARPGProjectileSpawner::SetSpawnLocation(FVector location)
{
	SpawnLocation = location;
}