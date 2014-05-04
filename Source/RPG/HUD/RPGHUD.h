// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "RPGHUD.generated.h"

/**
 * 
 */
UCLASS()
class ARPGHUD : public AHUD
{
	GENERATED_UCLASS_BODY()

	virtual void DrawHUD() OVERRIDE;
	
public:
	FVector2D CrosshairCenter;
	FVector CrosshairCenter3D;
	FVector Crosshair3DPos;
	void Get3DCrosshairPos(FVector2D CrosshairCen);
	float CrosshairAdjustedY;
	float AimDistance;
	UPROPERTY(BlueprintReadOnly, Category=HUDNative)
	class ARPGCharacter* CharOwner;
	TSharedPtr<class SRPGPowerInventoryWidget> PowerInventoryWidget;
	void ProjectCorsshairToHit(FVector hitLocation);
protected:

	void BuildHotbarWidget();
	void BuildPowerInventoryWidget();
	void BuildFloatingCombatTextWidget();

	TSharedPtr<class SRPGHotbarWidget> HotBarWidget;
	TSharedPtr<class SFloatingCombatTextWidget> FloatingCombatTextWidget;

	UFUNCTION(BlueprintCallable, Category = HUDHelpers)
	AActor* GetHUDHitActor();
	UFUNCTION(BlueprintCallable, Category = "RPGHUD|Helpers")
	float GetTargetHealth();


private:
	FVector GetStartLocation(const FVector& AimDir) const;
	FVector GetCameraAim() const;
	FHitResult HUDHitResult(const FVector& StartTrace, const FVector& EndTrace) const;
	
	class URPGAttributeComponent* TargetAttributeComponent;

	UTexture2D* CrosshairTex;
};
