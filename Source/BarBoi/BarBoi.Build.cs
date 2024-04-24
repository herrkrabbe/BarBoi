// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BarBoi : ModuleRules
{
	public BarBoi(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
		
		//Added this to use the navigation system classes
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem" });
		
		// Add required engine modules to the project's dependencies list. These modules are necessary for various functionalities within the game engine.
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject",
			"Engine", "InputCore", "HeadMountedDisplay",
			"GameplayTasks", "AIModule", "NavigationSystem" });
		
		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
