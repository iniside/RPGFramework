// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#include "RPG.h"
#include "RPGProjectile.h"
#include "RPGPlayerController.h"

ARPGProjectile::ARPGProjectile(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	CollisionSphereRadius = 5;
	ProjectileAcceleration 	= 1.04;
	ProjectileLifeTime 		= 10;
	CollisionComp = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(CollisionSphereRadius);
	CollisionComp->AlwaysLoadOnClient = true;
	CollisionComp->AlwaysLoadOnServer = true;
	CollisionComp->bTraceComplexOnMove = true;
	CollisionComp->BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->BodyInstance.SetResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionObjectType(ECC_Pawn);
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	RootComponent = CollisionComp;
	
	//Add Collision


	ParticleComp = PCIP.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("ParticleComp"));
	ParticleComp->bAutoActivate = true;
	ParticleComp->bAutoDestroy = false;
	ParticleComp->AttachParent = RootComponent;

	MovementComp = PCIP.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("ProjectileComp"));
	MovementComp->UpdatedComponent = CollisionComp;
	
	//Proj Speed
	MovementComp->InitialSpeed = 1000;
	MovementComp->MaxSpeed = 2000;
	
	MovementComp->bRotationFollowsVelocity = true;
	
	//add movement component

	//tick
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	

		
	//ProjectileMovementComponent Settings
	
	//No gravity
	MovementComp->ProjectileGravityScale = 0;
	
	//Bounces
	MovementComp->BounceVelocityStopSimulatingThreshold = 5; //uu/sec
	MovementComp->bShouldBounce = 0;
	MovementComp->Friction = 0;
	MovementComp->Bounciness=0;
}

void ARPGProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	//MovementComp->OnProjectileStop.AddDynamic(this, &ARPGProjectile::OnImpact);
	CollisionComp->MoveIgnoreActors.Add(Instigator);

	SetLifeSpan(10);
	MyController = Cast<ARPGPlayerController>(GetInstigatorController());
	
}

void ARPGProjectile::InitVelocity(FVector& ShootDirection)
{
	if (MovementComp)
	{
		MovementComp->Velocity = ShootDirection * MovementComp->InitialSpeed;
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Projectile Bounce Delegate
void ARPGProjectile::OnBounce(FHitResult const& HitResult)
{
	ARPGPlayerController* PC = Cast<ARPGPlayerController>(GetInstigatorController());
	if (PC) PC->ClientMessage("Bounce!!");
	
	//Get Hit Actor
	AActor* HitActor = HitResult.GetActor();
	if (!HitActor) return;
	
	//hit a pawn? Then do impact
	if (HitActor->IsA(APawn::StaticClass())) 
	{
		//do impact
		OnImpact(HitResult);
	}
}

//Projectile Stop Delegate
void ARPGProjectile::OnImpact(FHitResult const& HitResult)
{
	if (Role == ROLE_Authority && !bExploded)
	{
		Explode(HitResult);
		DisableAndDestroy();
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


void ARPGProjectile::Explode(const FHitResult& Impact)
{
	// effects and damage origin shouldn't be placed inside mesh at impact point
	const FVector NudgedImpactLocation = Impact.ImpactPoint + Impact.ImpactNormal * 10.0f;

	//Spawn Emitter At Impact Location
	if (ExplosionTemplate)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			Impact.GetActor(), 
			ExplosionTemplate, 
			NudgedImpactLocation, 
			Impact.ImpactNormal.Rotation(), 
			false); //auto destroy when explosion finishes
	}
	
	
	bExploded = true;
}

void ARPGProjectile::DisableAndDestroy()
{
	SetActorHiddenInGame(true);
	MovementComp->StopMovementImmediately();

	// give clients some time to show explosion
	SetLifeSpan( 1.0f );
}

void ARPGProjectile::OnRep_Exploded()
{
	static FName PowerTag = FName(TEXT("PowerTrace"));
	FCollisionQueryParams TraceParams(PowerTag, false);
	TraceParams.bTraceComplex = false;
	TraceParams.bTraceAsyncScene = false;
	TraceParams.bReturnPhysicalMaterial = true;
	FVector ProjDirection = GetActorRotation().Vector();

	const FVector StartTrace = GetActorLocation() - ProjDirection * 200;
	const FVector EndTrace = GetActorLocation() + ProjDirection * 150;
	FHitResult Impact;
	bool hitResult = GetWorld()->LineTraceSingle(Impact, StartTrace, EndTrace, ECollisionChannel::ECC_Pawn, FCollisionQueryParams(TEXT("ProjClient"), true, Instigator));
	if (!hitResult)
	{
		// failsafe
		Impact.ImpactPoint = GetActorLocation();
		Impact.ImpactNormal = -ProjDirection;
	}

	Explode(Impact);
}

void ARPGProjectile::PostNetReceiveVelocity(const FVector& NewVelocity)
{
	if (MovementComp)
	{
		MovementComp->Velocity = NewVelocity;
	}
}
//void ARPGProjectile::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(ARPGProjectile, bExploded);
//}
//```
void ARPGProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (bExploded) return;
	if (!MovementComp) return;
	//~~~~~~~~~~~~~~~~~~~~~~~~
	
	//Acceleration, server side only
	if (Role == ROLE_Authority)
	{
		/** Current velocity of moved component, replicated to clients using Actor ReplicatedMovement property. */
		MovementComp->Velocity *= ProjectileAcceleration;
	}	
}