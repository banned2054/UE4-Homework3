using UnrealBuildTool;
using System.Collections.Generic;

public class MultPlayerServerTarget : TargetRules
{

    public MultPlayerServerTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Server;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.Add("MultPlayer"); //根据你的项目名称更改此行
    }
}