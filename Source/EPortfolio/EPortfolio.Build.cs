// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EPortfolio : ModuleRules
{
	public EPortfolio(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AnimGraphRuntime", "EMultiplayerSessions", "OnlineSubsystem", "OnlineSubsystemSteam", "Niagara" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
