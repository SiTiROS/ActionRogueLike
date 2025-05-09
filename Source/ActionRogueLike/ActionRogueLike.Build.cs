// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class ActionRogueLike : ModuleRules
{
	public ActionRogueLike(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "AIModule", "GameplayTasks", "UMG", "GameplayTags",
			"AutomationController", "AutomationTest", "UnrealEd",
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[]
			{
				Path.Combine(ModuleDirectory, "Tests")
			}
		);

		/*if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.Add("EditorTestUtilities");
		}*/

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}