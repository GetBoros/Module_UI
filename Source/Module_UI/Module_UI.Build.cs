using UnrealBuildTool;

public class Module_UI : ModuleRules
{
	public Module_UI(ReadOnlyTargetRules Target) : base(Target)  // Make visible to Unreal build system
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "SlateCore" } );
	}
}
