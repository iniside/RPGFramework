#pragma once
#include "RPG.h"
#include "RPGCreateNewObject.generated.h"



UCLASS()
class URPGCreateNewObject : public UEdGraphNode
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintPure, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", FriendlyName = "Create Object From Blueprint", CompactNodeTitle = "Create", Keywords = "new create blueprint"), Category = Game)
		static UObject* NewObjectFromBlueprint(UObject* WorldContextObject, TSubclassOf<UObject> UC);


};

