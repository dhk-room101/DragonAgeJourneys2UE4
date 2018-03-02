// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DragonAgeJourneys : ModuleRules
{
	public DragonAgeJourneys(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
		PublicDependencyModuleNames.AddRange(new string[] { "XmlParser" });
        PublicDependencyModuleNames.AddRange(new string[] { "UMG", "Slate", "SlateCore" }); //for UMG
        PublicDependencyModuleNames.AddRange(new string[] { "MoviePlayer", "MediaAssets" }); // for loading screen
    }
}
