// Copyright 2026 Inspyr Studio, SAS. All Rights Reserved.

using UnrealBuildTool;

public class CygonLink : ModuleRules
{
	public CygonLink(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] { }
		);
				
		
		PrivateIncludePaths.AddRange(
			new string[] { }
		);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"UnrealEd",
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"AssetTools",
				"DirectoryWatcher",
			}
		);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[] { }
		);
	}
}
