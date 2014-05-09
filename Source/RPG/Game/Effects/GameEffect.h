// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Object.h"
#include "Tickable.h"
#include "../../RPGSystem/Effects/RPGEffectBase.h"
#include "GameEffect.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UGameEffect : public URPGEffectBase
{
	GENERATED_UCLASS_BODY()

	virtual void Tick(float DeltaTime)  OVERRIDE;


	virtual bool Initialize() OVERRIDE;
	
	virtual void AddEffect() OVERRIDE;
	
	virtual void SelfRemoveEffect() OVERRIDE;
	
	UPROPERTY(BlueprintReadOnly, Category="Effect|Attributes")
	class URPGAttributeBaseComponent* TargetAttributes;

protected:
	
	UPROPERTY(BlueprintReadOnly, Category = "Effect|Target|Attributes")
		float TargetHealth;
	UPROPERTY(BlueprintReadOnly, Category = "Effect|Target|Attributes")
		float TargetEnergy;
	UPROPERTY(BlueprintReadOnly, Category = "Effect|Target|Attributes")
		float TargetStamina;

private:
	void SetTargetAttributeUpdate();
};
