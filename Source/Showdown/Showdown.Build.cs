// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Showdown : ModuleRules
{
	public Showdown(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"Showdown",
			"Showdown/Variant_Platforming",
			"Showdown/Variant_Platforming/Animation",
			"Showdown/Variant_Combat",
			"Showdown/Variant_Combat/AI",
			"Showdown/Variant_Combat/Animation",
			"Showdown/Variant_Combat/Gameplay",
			"Showdown/Variant_Combat/Interfaces",
			"Showdown/Variant_Combat/UI",
			"Showdown/Variant_SideScrolling",
			"Showdown/Variant_SideScrolling/AI",
			"Showdown/Variant_SideScrolling/Gameplay",
			"Showdown/Variant_SideScrolling/Interfaces",
			"Showdown/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
