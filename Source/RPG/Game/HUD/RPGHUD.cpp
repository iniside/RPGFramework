// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#include "RPG.h"
#include "../RPGCharacter.h"
#include "../RPGPlayerController.h"
#include "RPGHUD.h"


ARPGHUD::ARPGHUD(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("Texture2D'/Game/SharedAssets/HUD/crosshair.crosshair'"));
	CrosshairTex = CrosshairTexObj.Object;
}

void ARPGHUD::ProjectCorsshairToHit(FVector hitLocation)
{
	if(!Canvas)
		return;

	CrosshairCenter3D += Canvas->Project(hitLocation);
}
void ARPGHUD::DrawHUD()
{
	Super::DrawHUD();

	// find center of the screen/Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	CrosshairCenter = Center;


	if(CrosshairTex && Canvas)
	{
		FVector2D CrosshairDrawStart(CrosshairCenter);
		CrosshairDrawStart.X -= CrosshairTex->GetSurfaceWidth() * 0.5f;
		CrosshairDrawStart.Y -= CrosshairTex->GetSurfaceHeight() * 0.5f;


		FCanvasTileItem TileItem (CrosshairDrawStart, CrosshairTex->Resource, FLinearColor::Red);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	}

}

FVector ARPGHUD::GetStartLocation(const FVector& AimDir) const
{
	ARPGPlayerController* PC = Cast<ARPGPlayerController>(GetOwningPlayerController());
	FVector OutStartTrace = FVector::ZeroVector;
	if(!GetOwningPawn())
		return OutStartTrace;

	if(PC)
	{

		FRotator UnusedRot;
		PC->GetPlayerViewPoint(OutStartTrace, UnusedRot);
		OutStartTrace = OutStartTrace + AimDir * ((GetOwningPawn()->GetActorLocation() - OutStartTrace) | AimDir);
		//OutStartTrace = OutStartTrace + AimDir * ((Instigator->GetActorLocation() - OutStartTrace));
	}
	return OutStartTrace;
}

FVector ARPGHUD::GetCameraAim() const
{
	ARPGPlayerController* PC = Cast<ARPGPlayerController>(GetOwningPlayerController());
	FVector FinalAim = FVector::ZeroVector;

	if(PC)
	{
		FVector CamLoc;
		FRotator CamRot;
		PC->GetPlayerViewPoint(CamLoc, CamRot);
		FinalAim = CamRot.Vector();
	}
	else if(GetOwningPawn())
	{
		FinalAim = GetOwningPawn()->GetBaseAimRotation().Vector();
	}
	return FinalAim;
}

FHitResult ARPGHUD::HUDHitResult(const FVector& StartTrace, const FVector& EndTrace) const
{
	FHitResult Hit(ForceInit);
	if(!GetOwningPawn())
		return Hit;


	static FName PowerTag = FName(TEXT("PowerTrace"));
	FCollisionQueryParams TraceParams(PowerTag, false, GetOwningPawn());
	TraceParams.bTraceComplex = false;
	TraceParams.bTraceAsyncScene = false;
	TraceParams.bReturnPhysicalMaterial = true;
	bool traceResult = GetWorld()->LineTraceSingle(Hit, StartTrace, EndTrace, ECollisionChannel::ECC_Pawn, TraceParams);
	return Hit;
}

AActor* ARPGHUD::GetHUDHitActor()
{
	AActor* actor = nullptr;

	//FVector Origin = GetCastingLocation();
	const FVector ShootDir = GetCameraAim();
	FVector StartTrace = GetStartLocation(ShootDir);

	const FVector EndTrace = (StartTrace + ShootDir * 10000.0f);
	FHitResult Impact = HUDHitResult(StartTrace, EndTrace);
	if (Impact.GetActor())
	{
		return Impact.GetActor();
	}
	return actor;
}

float ARPGHUD::GetTargetHealth()
{
	return 0;
}