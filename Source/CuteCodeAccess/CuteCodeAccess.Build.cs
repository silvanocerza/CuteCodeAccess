// Copyright 2018 Silvano Cerza All Rights Reserved.

using Microsoft.Win32;

namespace UnrealBuildTool.Rules
{
    public class CuteCodeAccess: ModuleRules
    {
        public CuteCodeAccess(ReadOnlyTargetRules Target) : base(Target)
        {
            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "Core",
                    "CoreUObject",
                    "Engine",
                    "SourceCodeAccess",
                    "DesktopPlatform",
                    "XmlParser",
                    "Projects",
                }
            );

            if (Target.bBuildEditor)
            {
                PrivateDependencyModuleNames.Add("HotReload");
            }

            PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        }
    }
}
