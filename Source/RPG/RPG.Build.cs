// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RPG : ModuleRules
{
	public RPG(TargetInfo Target)
	{
        PublicIncludePaths.AddRange(
        new string[] {
					    // ... add public include paths required here ...
				    }
        );
        PrivateIncludePaths.AddRange(
        new string[] {
					   // "Runtime/GameplayTags/Private",
					    // ... add other private include paths required here ...
				    }
        );
		PublicDependencyModuleNames.AddRange(new string[] 
        { 
            "Core", 
            "CoreUObject", 
            "Engine", 
            "InputCore",
            "UnrealEd",
            "GameplayTags"
        });
        
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate" });

        DynamicallyLoadedModuleNames.Add("EditorStyle");

        PrivateIncludePathModuleNames.AddRange(new string[]{
            "EditorStyle",
            "GameplayTags"
    });

        if (UEBuildConfiguration.bBuildEditor == true)
        {
            //PrivateDependencyModuleNames.Add("GameplayTagsEditor");
        }
    }
}
