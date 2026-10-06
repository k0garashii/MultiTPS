// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MultiGAS : ModuleRules
{
	public MultiGAS(ReadOnlyTargetRules Target) : base(Target)
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

		PrivateDependencyModuleNames.AddRange(new string[] { "GameplayAbilities" });

		PublicIncludePaths.AddRange(new string[] {
			"MultiGAS",
			"MultiGAS/Variant_Platforming",
			"MultiGAS/Variant_Platforming/Animation",
			"MultiGAS/Variant_Combat",
			"MultiGAS/Variant_Combat/AI",
			"MultiGAS/Variant_Combat/Animation",
			"MultiGAS/Variant_Combat/Gameplay",
			"MultiGAS/Variant_Combat/Interfaces",
			"MultiGAS/Variant_Combat/UI",
			"MultiGAS/Variant_SideScrolling",
			"MultiGAS/Variant_SideScrolling/AI",
			"MultiGAS/Variant_SideScrolling/Gameplay",
			"MultiGAS/Variant_SideScrolling/Interfaces",
			"MultiGAS/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
