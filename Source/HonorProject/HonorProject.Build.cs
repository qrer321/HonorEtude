// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HonorProject : ModuleRules
{
	public HonorProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Sockets", "Networking", "UMG", "Slate", "SlateCore" });
	}
}
