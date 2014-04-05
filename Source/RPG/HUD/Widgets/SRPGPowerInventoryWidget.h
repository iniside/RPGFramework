// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "../../Powers/RPGPowerBase.h"
#include "RPG.h"
#include "Slate.h"
/**
 * 
 */
//UCLASS()
class SRPGPowerInventoryWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRPGPowerInventoryWidget) //this is really important it must be declared
	{}

	/* Owning HUD for getting Game World */
	SLATE_ARGUMENT(TWeakObjectPtr<class ARPGHUD>, OwnerHUD) //here we set argument that is accessible outside of slate as well as with InArgs
	SLATE_ARGUMENT(TArray<TWeakObjectPtr<URPGPowerBase>>, PowerListArg) 
	SLATE_DEFAULT_SLOT(FArguments, Content) //and this, but to be honest I'm not sure what it does at this point
	SLATE_END_ARGS() //this is obviously end
	void Construct(const FArguments& InArgs); //i assume here we will build body of widget that will be drawed on screen
	TSharedPtr<STileView<TWeakObjectPtr<URPGPowerBase>>> TileView;
	TArray<TWeakObjectPtr<URPGPowerBase>> PowersList;
	/*
		we will use it in RPGHUD to see all powers that are in possesion of Pawn! 
		Please note that controllers do not have any power list. Only pawn can have powers!
		Controller can only execute them.
	*/

	void Tick( const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime ) OVERRIDE;
	FString GetCharacterHealth();
	void SetPower(TWeakObjectPtr<URPGPowerBase> power);
	TWeakObjectPtr<URPGPowerBase> Powar;
protected:
	TSharedRef<ITableRow> SOnGenerateWidgetForList(TWeakObjectPtr<URPGPowerBase> Item, const TSharedRef< STableViewBase >& OwnerTable);
	FSlateColor	GetTextColor() const;
	/* onwer of our hud */
	TWeakObjectPtr<class ARPGHUD> OwnerHUD;
	
	float Health;
	float GetHealth();
};
