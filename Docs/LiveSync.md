# Live Sync Workflow

Cygon Link supports **real-time synchronization** between Cygon and Unreal Engine. When you modify and re-export a scene in Cygon, all corresponding assets in your UE level update automatically — no manual reimport needed.

## Setup

1. Complete the [First Import](FirstImport.md) for your scene.
2. Add the imported asset to your level.
3. Keep Unreal Engine open in the background.

## Enabling Auto-Reimport in Unreal Engine

Unreal Engine's auto-reimport must be active for live sync to work.

1. Go to **Edit → Editor Preferences**.
2. Search for **Auto Reimport**.
3. Enable **Monitor Content Directories**.
4. Make sure the `Content/` folder of your project is in the monitored directories list.

## Live Editing Loop

Once set up, the workflow is:

1. Open your scene in **Cygon**.
2. Make changes — modify geometry, adjust transforms, update materials.
3. Export with **CTRL + S** or the Cygon export button.
4. Switch back to **Unreal Engine**.
5. The auto-reimport system detects the changed `.usda` file and triggers Cygon Link's reimport pipeline.
6. All assets and their instances in the level update silently and automatically.

## What Updates Automatically

| Asset Type | Updates Automatically |
|---|---|
| Static Mesh geometry | ✅ |
| Transforms / positions | ✅ |
| Materials | ✅ |
| Level instances | ✅ |

## Notes

- The reimport is silent — no dialog will appear if the import succeeds.
- A log entry is written to the **Output Log** (`LogTemp`) for each reimported asset.
- If an export produces no changes, no reimport is triggered.
