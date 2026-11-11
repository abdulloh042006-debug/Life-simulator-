// Copyright Dredd 2026. All rights reserved.

using UnrealBuildTool;

public class InteractionSystem : ModuleRules
{
    public InteractionSystem(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        CppStandard = CppStandardVersion.Cpp20;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "UMG"
        });
    }
}
