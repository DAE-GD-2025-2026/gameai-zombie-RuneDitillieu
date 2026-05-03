// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class GameAI_ZombieTarget : TargetRules
{
	public GameAI_ZombieTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;
		ExtraModuleNames.Add("GameAI_Zombie");
		
		// // Enable loading external plugin modules at runtime
		// LinkType = TargetLinkType.Modular;
	}
}
