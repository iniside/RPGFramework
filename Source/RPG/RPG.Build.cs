// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RPG : ModuleRules
{
	public RPG(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
        
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate" });

        DynamicallyLoadedModuleNames.Add("EditorStyle");

        PrivateIncludePathModuleNames.Add("EditorStyle");
    }
}
