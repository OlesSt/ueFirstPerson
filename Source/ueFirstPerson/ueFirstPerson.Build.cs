// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ueFirstPerson : ModuleRules
{
	public ueFirstPerson(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AkAudio" });
	}
}
