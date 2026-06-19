# Installation

> **New to Cygon?** Before installing the plugin, make sure Cygon is installed and running on your machine. Watch the [Cygon installation tutorial on YouTube](https://www.youtube.com/watch?v=SaS8J_4AumM) to get started.

## Option 1 — Via Fab (Recommended)
- Go to the [Cygon Link page on Fab](https://www.fab.com/) (soon).
- Add the plugin to your Library.
- Open the **Epic Games Launcher**, go to **Unreal Engine → Library**.
- Scroll down to **Fab Library**, find **Cygon Link**, and click **Install to Engine**.
- Select your engine version and confirm.
- Open your project, go to **Edit → Plugins**, search for **Cygon Link**, enable it, and restart the editor.

> For general information on managing installed plugins, refer to the [Epic Games Plugin Documentation](https://dev.epicgames.com/documentation/unreal-engine/working-with-plugins-in-unreal-engine).

## Option 2 — Via Git (Source)
- Navigate to your Unreal project's root folder.
- Create a `Plugins/` folder if it does not already exist.
- Clone the repository into it:
```shell
git clone https://github.com/inspyrstudio/CygonLinkForUnrealEngine
```
- Right-click your .uproject file and select Generate Visual Studio project files (or your preferred IDE equivalent).
- Open your project in Unreal Engine and click Yes when prompted to rebuild the missing modules, or build it directly from your IDE.

## Verifying the Installation
After installation and editor restart:
- Go to **Edit → Plugins** and confirm **Cygon Link** appears and is enabled.
- Confirm **USD Importer** is also enabled (see [Requirements](Requirements.md)).
- Drag the included [sample scene](Samples/SampleScene/RestlessPeak.usda) into your **Content Browser** to verify the plugin intercepts and imports it correctly.
> **Note:** For more information on importing your own scenes from Cygon, refer to the [First Import Guide](FirstImport.md).