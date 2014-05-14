// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "RPG.h"
#include "RPGAbilityManagerComponent.generated.h"

//DECLARE_DELEGATE_RetVal(URPGPowerBase, FOnNewPowerAdded);
/**
 * 
 */
UCLASS()
class URPGAbilityManagerComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

	/* we might want to make it private or protected and do not expose to blueprint
	and only interaction troug equip/remove/get power/s */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ability|Manager")
	TArray<class URPGAbilityBase*> AbilityList;

	/*
	Stub function. Just for testing, probably will need change o removed, do not overuse it!
	*/
	UFUNCTION(BlueprintCallable, Category="Ability|Manager")
		void AddPower(TSubclassOf<class URPGAbilityBase> ability, APawn* Owner);



public:
	class URPGAbilityBase* GetButtonOne();
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Ability")
	class URPGAbilityBase* AbilityButtonOneObj;
};