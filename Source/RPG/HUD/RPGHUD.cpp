// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#include "RPG.h"
#include "../RPGCharacter.h"
#include "../RPGPlayerController.h"
#include "Widgets/SRPGHotbarWidget.h"
#include "Widgets/SRPGPowerInventoryWidget.h"
#include "Widgets/SFloatingCombatTextWidget.h"
#include "../Components/RPGPowerManagerComponent.h"
#include "RPGHUD.h"


ARPGHUD::ARPGHUD(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("Texture2D'/Game/SharedAssets/HUD/crosshair.crosshair'"));
	CrosshairTex = CrosshairTexObj.Object;
}

void ARPGHUD::Get3DCrosshairPos(FVector2D CrosshairCen)
{
	if(!Canvas)
	{
		return;
	}

	FVector WorldDirectionOfCrosshair2D;
	Canvas->Deproject(CrosshairCen, Crosshair3DPos, WorldDirectionOfCrosshair2D);

	Crosshair3DPos += WorldDirectionOfCrosshair2D * AimDistance;
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
	//CrosshairCenter.Y += (Canvas->ClipY*0.03);
	//CrosshairAdjustedY = (Canvas->ClipY*0.03);
	//Get3DCrosshairPos(CrosshairCenter);

	if(CrosshairTex && Canvas)
	{
		FVector2D CrosshairDrawStart(CrosshairCenter);
		CrosshairDrawStart.X -= CrosshairTex->GetSurfaceWidth() * 0.5f;
		CrosshairDrawStart.Y -= CrosshairTex->GetSurfaceHeight() * 0.5f;

		//FVector2D CrosshairDrawStart(CrosshairCenter);
		//CrosshairCenter3D.X -= CrosshairTex->GetSurfaceWidth() * 0.5f;
		//CrosshairCenter3D.Z -= CrosshairTex->GetSurfaceHeight() * 0.5f;

		FCanvasTileItem TileItem (CrosshairDrawStart, CrosshairTex->Resource, FLinearColor::Red);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	}
	BuildFloatingCombatTextWidget();
	BuildHotbarWidget();
	BuildPowerInventoryWidget();
	
	//const FVector2D CrosshairDrawPosition(Center.X - (CrosshairTex->GetSurfaceWidth()*0.5f), (Center.Y - (CrosshairTex->GetSurfaceHeight()*0.5f - (Canvas->ClipY*0.05))));

	//FCanvasTileItem TileItem (CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	//TileItem.BlendMode = SE_BLEND_Translucent;
	//Canvas->DrawItem(TileItem);
}


void ARPGHUD::BuildHotbarWidget()
{
	if(!GEngine || !GEngine->GameViewport)
	{
		return;
	}

	if(!HotBarWidget.IsValid())
	{
		const ARPGPlayerController* PC = Cast<ARPGPlayerController>(PlayerOwner);
		if(PC)
		{
			SAssignNew(HotBarWidget, SRPGHotbarWidget).OwnerHUD(this);
			if(HotBarWidget.IsValid())
			{
				GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(HotBarWidget.ToSharedRef()));

			}
		}
	}
}

void ARPGHUD::BuildPowerInventoryWidget()
{
	if(!GEngine || !GEngine->GameViewport)
	{
		return;
	}

	if(!PowerInventoryWidget.IsValid())
	{
		const ARPGPlayerController* PC = Cast<ARPGPlayerController>(PlayerOwner);
		const ARPGCharacter* MyPawn = Cast<ARPGCharacter>(GetOwningPawn());
		
		CharOwner = CastChecked<ARPGCharacter>(GetOwningPawn());
		if(PC)
		{
			if(MyPawn)
			{
				SAssignNew(PowerInventoryWidget, SRPGPowerInventoryWidget).OwnerHUD(this).PowerListArg(MyPawn->PowerManager->PowerListSlate);
			}
			//SAssignNew(PowerInventoryWidget, SRPGPowerInventoryWidget).OwnerHUD(this);
			if(PowerInventoryWidget.IsValid())
			{
				GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(PowerInventoryWidget.ToSharedRef()));
			}
		}
	}
}

void ARPGHUD::BuildFloatingCombatTextWidget()
{
	if(!GEngine || !GEngine->GameViewport)
	{
		return;
	}

	if(!PowerInventoryWidget.IsValid())
	{
		const ARPGPlayerController* PC = Cast<ARPGPlayerController>(PlayerOwner);
		const ARPGCharacter* MyPawn = Cast<ARPGCharacter>(GetOwningPawn());
		CharOwner = CastChecked<ARPGCharacter>(GetOwningPawn());
		if(PC)
		{
			//if(MyPawn)
			//{
				SAssignNew(FloatingCombatTextWidget, SFloatingCombatTextWidget).OwnerHUD(this);
			//}
			//SAssignNew(PowerInventoryWidget, SRPGPowerInventoryWidget).OwnerHUD(this);
			if(FloatingCombatTextWidget.IsValid())
			{
				GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(FloatingCombatTextWidget.ToSharedRef()));
			}
		}
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