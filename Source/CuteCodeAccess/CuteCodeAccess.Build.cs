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
                    "SourceCodeAccess",
                    "DesktopPlatform",
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
