# Cygon Link for Unreal Engine
Seamless integration and live-sync for USDA files.

Cygon Link is a powerful bridge that provides a robust pipeline for bringing USDA (Universal Scene Description) assets into Unreal Engine. It features a custom Factory for automated conversion. It also syncs changes from Cygon to Unreal Engine in real-time.

## Key Features
📦 **Automated Asset Generation**: Converts USDA hierarchies, meshes, and materials directly into native Unreal Engine assets (Static Meshes, Materials, etc.) with collision support.

🔥 **Live Hot-Reloading / Reimport**: Detects file changes in Cygon and instantly updates all instances in your active Unreal Engine level.

🛠️ **Intelligent Mesh Processing**: Uses Unreal Engine's native USD Importer under the hood while applying custom processing to ensure perfect results.

## Documentation

Full documentation is available in the [`Docs/`](Docs/) folder:

| Guide | Description |
|---|---|
| [Prerequisites](Docs/Prerequisites.md) | Software requirements and enabling the USD Importer plugin |
| [Installation](Docs/Installation.md) | Installing via Fab or from source |
| [How It Works](Docs/HowItWorks.md) | Technical overview of the import pipeline |
| [First Import](Docs/FirstImport.md) | Step-by-step guide for your first Cygon scene import |
| [Live Sync](Docs/LiveSync.md) | Setting up real-time sync between Cygon and UE |
| [Troubleshooting](Docs/Troubleshooting.md) | Common issues and fixes |

A sample `.usda` file is included at [`Docs/CygonSample.usda`](Docs/CygonSample.usda) to verify the plugin works without needing Cygon installed.

## Getting Started

### Installation via Fab (Pre-compiled)
The easiest way to get started is to get the pre-compiled plugin directly from Fab.

1. Go to the [Cygon Link page on Fab](https://www.fab.com/) *(soon)*.
2. Add the plugin to your library and install it to your current Unreal Engine version.
3. Open your Unreal Engine project, go to **Edit > Plugins**, search for **Cygon Link**, and check the box to enable it.
4. Restart the editor if prompted.

*(For general information on managing installed plugins, refer to the [Epic Games Plugin Documentation](https://dev.epicgames.com/documentation/unreal-engine/working-with-plugins-in-unreal-engine)).*

### Installation via Git (Source Code)
If you prefer to compile the plugin yourself or need the latest development version:

1. Navigate to your Unreal project's root folder.
2. Ensure you have a `Plugins` folder, or create one if it doesn't exist.
3. Open a terminal inside the `Plugins` folder and clone the repository:
   ```shell
   git clone https://github.com/inspyrstudio/CygonLinkForUnrealEngine
   ```
4. Right-click your .uproject file and select Generate Visual Studio project files (or your preferred IDE equivalent).
5. Open your project in Unreal Engine and click Yes when prompted to rebuild the missing modules, or build it directly from your IDE.

## How to use it?
1. Import your Assets: In Cygon export your level in the `Content` folder of your Unreal project. This will ensure that the plugin can find the files and reimport them correctly. To export you can use CTRL + S or the export button in the UI.
2. Add to Level: Drag the imported asset from the Content Browser into your Level Viewport or Outliner.
3. Live Editing Workflow: Keep your Unreal Engine Scene open. Open the source file in Cygon. Modify geometry or transforms and Export or Quick Export with CTRL + S.
4. Switch back to Unreal Engine: the auto-reimport process will trigger, and your meshes will update instantly.

## Dependencies
This plugin has no third-party code or libraries. The only dependency is
**USDImporter**, which is an Epic Games built-in Unreal Engine plugin
(shipped with the engine itself).

## Miscellaneous
⚙️ Requirements
Unreal Engine Version: 5.7 or higher (Tested on recent UE5.7 versions). Ensure the USD Importer plugin is enabled in your Unreal project.
Cygon Version: 0.2.0.7i minimum. (Note: versions prior to 0.2.0.7i do not support the proper export pipeline needed for engine integration).

🤝 Contributing
Contributions are welcome! Please feel free to tell us about anything that doesn't work with the package on the Discord.