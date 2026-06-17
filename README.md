# Cygon Link for Unreal Engine
Seamless integration and live-sync for USDA files.

Cygon Link is a powerful bridge that provides a robust pipeline for bringing USDA (Universal Scene Description) assets into Unreal Engine. It features a custom Factory for automated conversion. It also syncs changes from Cygon to Unreal Engine in real-time.

## Key Features
- **Automated Asset Generation**: Converts USDA hierarchies, meshes, and materials directly into native Unreal Engine assets (Static Meshes, Materials, etc.) with collision support.
- **Live Hot-Reloading / Reimport**: Detects file changes in Cygon and instantly updates all instances in your active Unreal Engine level.
- **Intelligent Mesh Processing**: Uses Unreal Engine's native USD Importer under the hood while applying custom processing to ensure perfect results.

## Documentation

Full documentation is available in the [`Docs/`](Docs/) folder:

| Guide                                | Description                                                |
|--------------------------------------|------------------------------------------------------------|
| [Requirements](Docs/Requirements.md) | Software requirements and enabling the USD Importer plugin |
| [Installation](Docs/Installation.md) | Installing via Fab or from source                          |
| [FirstImport](Docs/FirstImport.md)   | Step-by-step guide for your Cygon scene import        |
| [Live Sync](Docs/LiveSync.md)        | Setting up real-time sync between Cygon and UE             |

A sample `.usda` file is included at [Docs/Samples](Docs/Samples) to verify the plugin works without needing Cygon installed.

## Getting Started
### Installation via Fab (Pre-compiled)
The easiest way to get started is to get the pre-compiled plugin directly from Fab.

- Go to the [Cygon Link page on Fab](https://www.fab.com/) *(soon)*.
- Add the plugin to your library and install it to your current Unreal Engine version.
- Open your Unreal Engine project, go to **Edit > Plugins**, search for **Cygon Link**, and check the box to enable it.
- Restart the editor if prompted.

*(For general information on managing installed plugins, refer to the [Epic Games Plugin Documentation](https://dev.epicgames.com/documentation/unreal-engine/working-with-plugins-in-unreal-engine)).*

### Installation via Git (Source Code)
If you prefer to compile the plugin yourself or need the latest development version:

- Navigate to your Unreal project's root folder.
- Ensure you have a `Plugins` folder, or create one if it doesn't exist.
- Open a terminal inside the `Plugins` folder and clone the repository:
   ```shell
   git clone https://github.com/inspyrstudio/CygonLinkForUnrealEngine
   ```
- Right-click your .uproject file and select Generate Visual Studio project files (or your preferred IDE equivalent).
- Open your project in Unreal Engine and click Yes when prompted to rebuild the missing modules, or build it directly from your IDE.

## How to use it?
- **Import your assets**: In Cygon, export your level into the `Content/` folder of your Unreal project (CTRL + S or the export button). As long as the editor is open, Cygon Link detects the new files and imports them automatically — no pop-up, and no editor preferences to enable.
- **Add to Level**: Drag the imported asset from the Content Browser into your Level Viewport or Outliner.
- **Live Editing Workflow**: Keep your Unreal Engine project open. Open the source file in Cygon, modify geometry or transforms, and Export / Quick Export with CTRL + S.
- **Switch back to Unreal Engine**: Cygon Link detects the change and reimports automatically — your meshes update without any manual step.

## Dependencies
This plugin has no third-party code or libraries. The only dependency is
**USDImporter**, which is an Epic Games built-in Unreal Engine plugin
(shipped with the engine itself).

## Miscellaneous
- Requirements
Unreal Engine Version: 5.7 or higher (Tested on recent UE5.7 versions). Ensure the USD Importer plugin is enabled in your Unreal project.
Cygon Version: 0.2.3i or higher. (Note: versions prior to 0.2.3i do not support the proper export pipeline needed for engine integration).

- Contributing
Contributions are welcome! Please feel free to tell us about anything that doesn't work with the package on the Discord.
