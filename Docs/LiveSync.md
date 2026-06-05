# Live Sync Workflow

Cygon Link supports **real-time synchronization** between Cygon and Unreal Engine. When you modify and re-export a scene in Cygon, all corresponding assets in your UE level update automatically — no manual reimport needed.

## Setup
- Read the [Import](Import.md) for your scene.
- Add the imported asset to your level.
- Keep Unreal Engine open in the background.

## Enabling Auto-Reimport in Unreal Engine
Unreal Engine's auto-reimport must be active for a better experience.
- Go to **Edit → Editor Preferences**.
- Search for **Auto Reimport**.
- Enable **Monitor Content Directories**.
- Make sure the `Content/` folder of your project is in the monitored directories list.

## Live Editing Loop
Once set up, the workflow is:
- Open your scene in **Cygon**.
- Make changes — modify geometry.
- Export with **CTRL + S** or the Cygon export button.
- Switch back to **Unreal Engine**.
- The auto-reimport system detects the changed `.usda` file and triggers Cygon Link's reimport pipeline.
- All assets and their instances in the level update silently and automatically (silently only if auto-reimport is enabled).
