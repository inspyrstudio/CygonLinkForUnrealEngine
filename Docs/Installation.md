# Installation

## Option 1 — Via Fab (Recommended)

1. Go to the [Cygon Link page on Fab](https://www.fab.com/).
2. Add the plugin to your library.
3. Open the **Epic Games Launcher**, go to **Unreal Engine → Library**.
4. Scroll down to **Vault**, find **Cygon Link**, and click **Install to Engine**.
5. Select your engine version and confirm.
6. Open your project, go to **Edit → Plugins**, search for **Cygon Link**, enable it, and restart the editor.

## Option 2 — Via Git (Source)

1. Navigate to your Unreal project's root folder.
2. Create a `Plugins/` folder if it does not already exist.
3. Clone the repository into it:
   ```shell
   git clone https://github.com/inspyrstudio/CygonLinkForUnrealEngine
   ```
4. Right-click your `.uproject` file and select **Generate Visual Studio project files**.
5. Open the project in Unreal Engine and click **Yes** when prompted to rebuild missing modules, or build directly from your IDE.

## Verifying the Installation

After installation and editor restart:

1. Go to **Edit → Plugins** and confirm **Cygon Link** appears and is enabled.
2. Confirm **USD Importer** is also enabled (see [Prerequisites](Prerequisites.md)).
3. Drag the included [CygonSample.usda](CygonSample.usda) file into your **Content Browser** to verify the plugin intercepts and imports it correctly.
