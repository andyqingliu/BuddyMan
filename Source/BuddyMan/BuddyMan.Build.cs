// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BuddyMan : ModuleRules
{
	public BuddyMan(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				"BuddyMan"
			}
		);

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "Slate", "SlateCore", "HeadMountedDisplay" });
		
		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"GameplayAbilities",
				"GameplayTags",
				"GameplayTasks"
			}
		);
	}
}
