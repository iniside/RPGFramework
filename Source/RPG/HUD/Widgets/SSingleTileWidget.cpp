// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#include "RPG.h"
#include "SSingleTileWidget.h"

/**
* 1. Draw something on screen. - done
* 2. Draw it in right position.
* 3. Assign keyboard event.
* 4. Make shortcut to keybord external, not hardcoded
* 5. Assign Power to button (power will just spawn debug trace lines)
* 6. Add drag&end drop of interace or select ui, so user can assign any power to button.
* 7. Refactor above shit, to make hotbar, and not make this class to big (;.
*/
void SSingleTileWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;
}

FSlateColor	SSingleTileWidget::GetTextColor() const
{
	const FLinearColor GoldColor = FLinearColor(FColor(255,213,160,255));
	return GoldColor;
}