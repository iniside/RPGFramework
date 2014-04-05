// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#include "RPG.h"
#include "../../RPGCharacter.h"
#include "../../Powers/RPGPowerBase.h"
#include "../RPGHUD.h"
#include "SFloatingCombatTextWidget.h"

/**
* 1. Draw something on screen. - done
* 2. Draw it in right position.
* 3. Assign keyboard event.
* 4. Make shortcut to keybord external, not hardcoded
* 5. Assign Power to button (power will just spawn debug trace lines)
* 6. Add drag&end drop of interace or select ui, so user can assign any power to button.
* 7. Refactor above shit, to make hotbar, and not make this class to big (;.
*/
void SFloatingCombatTextWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;

		ChildSlot
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			SNew(SOverlay)
			+SOverlay::Slot()
			.HAlign(EHorizontalAlignment::HAlign_Left)
			.VAlign(EVerticalAlignment::VAlign_Center)
			[
				SNew(STextBlock)
				.ColorAndOpacity(this, &SFloatingCombatTextWidget::GetTextColor)
				.Text(this, &SFloatingCombatTextWidget::GetTargetsHealth)
			]
		];
}

FSlateColor	SFloatingCombatTextWidget::GetTextColor() const
{
	const FLinearColor GoldColor = FLinearColor(FColor(255,0,0,255));
	return GoldColor;
}

void SFloatingCombatTextWidget::Tick( const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime )
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
}


FString SFloatingCombatTextWidget::GetTargetsHealth() const
{
	TWeakObjectPtr<ARPGCharacter> MyPawn = Cast<ARPGCharacter>(OwnerHUD->CharOwner);
	MyPawn->ActionButtonOneObj->GetCurrentCastTime();
	if(MyPawn->ActionButtonOneObj->CasTimeDelegate.IsBound())
	{
		return FString::FormatAsNumber(MyPawn->ActionButtonOneObj->CasTimeDelegate.Execute());
	}
	return FString::FormatAsNumber(0.0f);
	//return FString::FormatAsNumber(MyPawn->DamageCaused);
}