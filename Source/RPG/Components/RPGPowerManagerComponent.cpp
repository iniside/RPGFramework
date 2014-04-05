// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#include "RPG.h"
#include "../RPGCharacter.h"
//#include "../HUD/RPGHUD.h"
//#include "../HUD/Widgets/SRPGPowerInventoryWidget.h"
#include "RPGPowerManagerComponent.h"
#include "../RPGPlayerController.h"


URPGPowerManagerComponent::URPGPowerManagerComponent(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	
}

void URPGPowerManagerComponent::AddPower(TSubclassOf<URPGPowerBase> power)
{
	if(power)
	{
		URPGPowerBase* powerObj = ConstructObject<URPGPowerBase>(power);
		ARPGCharacter* MyPawn = Cast<ARPGCharacter>(GetOwner());
		if(MyPawn)
		{
			URPGEquipmentManagerComponent* comp = MyPawn->EquipmentManager.Get();
			
			powerObj->SetPowerOner(MyPawn);
			//powerObj->SetMainCasterWeapon(MyPawn->EquipmentManager->MainWeapon);
			//powerObj->SetMainCasterWeapon(comp->MainWeapon);
			PowerList.AddUnique(powerObj);
			PowerListSlate.AddUnique(powerObj);
			//ARPGPlayerController* MyPC = Cast<ARPGPlayerController>(MyPawn->GetController());
			//ARPGHUD* const MyHUD = (MyPC) ? Cast<ARPGHUD>(MyPC->GetHUD()) : NULL;
			////ARPGHUD* hud  = Cast<ARPGHUD>(MyPC->GetHUD());
			////MyHUD->PowerInventoryWidget->TileView->RequestListRefresh();
			//if(MyHUD)
			//{
			//	if(MyHUD->PowerInventoryWidget.IsValid())
			//	{
			//		SRPGPowerInventoryWidget* const widget = (MyHUD) ? MyHUD->PowerInventoryWidget.Get() : NULL;
			//		widget->PowersList.AddUnique(powerObj);
			//		widget->TileView->RequestListRefresh();
			//	}
			//}
			//hack, this should be used, but since we have no GUI to make work,
			//we will resolve to this for time being.
			if(!MyPawn->ActionButtonOneObj)
			{
				MyPawn->ActionButtonOneObj = powerObj;
			}
			else if (!MyPawn->ActionButtonTwoObj)
			{
				MyPawn->ActionButtonTwoObj = powerObj;
			}
		}
	}
	//ARPGCharacter* MyPawn = Cast<ARPGCharacter>(GetOwner());
	//		FActorSpawnParameters SpawnInfo;
	//	SpawnInfo.bNoCollisionFail = true;
	//	URPGPowerBase* powerObj = GetWorld()->SpawnActor<URPGPowerBase>(power, SpawnInfo);
	//	powerObj->PowerOwner = MyPawn;
	//	powerObj->SetOwner(MyPawn);
	//	powerObj->Instigator = MyPawn;
	//		powerObj->MainCasterWeapon = MyPawn->EquipmentManager->MainWeapon;
	//		PowerList.AddUnique(powerObj);
}

void URPGPowerManagerComponent::DrawInventoryGUI()
{
}