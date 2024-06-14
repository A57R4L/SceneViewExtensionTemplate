// Custom SceneViewExtension Template for Unreal Engine
// Copyright 2023 - 2024 Ossi Luoto

using UnrealBuildTool;
using System.IO;

public class SceneViewExtensionTemplate : ModuleRules
{
    public SceneViewExtensionTemplate(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "RenderCore",
            "Renderer",
            "RHI",
            "Projects",
            });

        var EngineDir = Path.GetFullPath(Target.RelativeEnginePath);

        PrivateIncludePaths.AddRange(
            new string[] {
				// this is required to find PostProcessing includes f.ex. screenpass.h
				Path.Combine(EngineDir, "Source/Runtime/Renderer/Private")
            }
        );
    }
}
