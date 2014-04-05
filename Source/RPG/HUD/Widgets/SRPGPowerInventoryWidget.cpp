// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#include "RPG.h"
#include "../RPGHUD.h"
#include "../../RPGCharacter.h"
#include "../../Components/RPGPowerManagerComponent.h"
#include "SRPGPowerInventoryWidget.h"

/**
* 1. Draw something on screen. - done
* 2. Draw it in right position.
* 3. Assign keyboard event.
* 4. Make shortcut to keybord external, not hardcoded
* 5. Assign Power to button (power will just spawn debug trace lines)
* 6. Add drag&end drop of interace or select ui, so user can assign any power to button.
* 7. Refactor above shit, to make hotbar, and not make this class to big (;.
*/
void SRPGPowerInventoryWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;
	//PowersList = InArgs._PowerListArg;
	ARPGCharacter* MyPawn = Cast<ARPGCharacter>(OwnerHUD->CharOwner);
	PowersList = MyPawn->PowerManager->PowerListSlate;
	//if(Powar.IsValid())
	//{
		ChildSlot
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			[
				SNew(SOverlay)
				+SOverlay::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Center)
				.VAlign(EVerticalAlignment::VAlign_Top)
				[
					//SNew(STextBlock)
					//.ColorAndOpacity(this, &SRPGPowerInventoryWidget::GetTextColor)
					//.Text(this, &SRPGPowerInventoryWidget::GetCharacterHealth)
					//SNew(STileView<TWeakObjectPtr<URPGPowerBase>>)
					SAssignNew(TileView, STileView<TWeakObjectPtr<URPGPowerBase>>)
					.ListItemsSource(&PowersList)
					//.OnGenerateTile(STileView<URPGPowerBase*>::GenerateNewWidget(&SRPGPowerInventoryWidget::GetPower ))
					.OnGenerateTile(this, &SRPGPowerInventoryWidget::SOnGenerateWidgetForList)
				]

			];
	//}
}
TSharedRef<ITableRow> SRPGPowerInventoryWidget::SOnGenerateWidgetForList(TWeakObjectPtr<URPGPowerBase> Item, const TSharedRef< STableViewBase >& OwnerTable)
{
	 TSharedRef< STableRow<TWeakObjectPtr<URPGPowerBase> > > ReturnRow = SNew(STableRow<TWeakObjectPtr<URPGPowerBase> >  , OwnerTable)
       .Content()
	   [
			SNew(STextBlock)
			.Text(FString(Item->GetDescription()))
	   ];
	 //TileView->RequestListRefresh();
    return ReturnRow;
}


void SRPGPowerInventoryWidget::SetPower(TWeakObjectPtr<URPGPowerBase>  power)
{
	Powar = power;
}

void SRPGPowerInventoryWidget::Tick( const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime )
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
	//ARPGCharacter* MyPawn = Cast<ARPGCharacter>(OwnerHUD->CharOwner);
	//PowersList = MyPawn->PowerManager->PowerListSlate;
	//TileView->RequestListRefresh();

}
//this, &SRPGPowerInventoryWidget::GetCharacterHealth
float SRPGPowerInventoryWidget::GetHealth()
{
	ARPGCharacter* MyPawn = Cast<ARPGCharacter>(OwnerHUD->CharOwner);
	if(MyPawn)
	{
		return MyPawn->GetCurrentHealth();
	}
	return 0;
}
FString SRPGPowerInventoryWidget::GetCharacterHealth()
{
	ARPGCharacter* MyPawn = Cast<ARPGCharacter>(OwnerHUD->CharOwner);
	if(MyPawn)
	{
		return FString::FormatAsNumber(MyPawn->GetCurrentHealth());
	}
	return FString("asd");
}

FSlateColor	SRPGPowerInventoryWidget::GetTextColor() const
{
	const FLinearColor GoldColor = FLinearColor(FColor(255,213,160,255));
	return GoldColor;
}