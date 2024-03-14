// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Battlebots : ModuleRules
{
	public Battlebots(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "GameplayAbilities", "GameplayTags", "GameplayTasks", "UMG"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"OnlineSubsystem", "OnlineSubsystemSteam", "SteamSockets", "CoreOnline", "OnlineSubsystemNull"
		});
	}
}