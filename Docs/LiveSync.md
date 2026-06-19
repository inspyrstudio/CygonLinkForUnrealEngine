# Live Sync Workflow

Cygon Link supports **real-time synchronization** between Cygon and Unreal Engine. When you modify and re-export a scene in Cygon, all corresponding assets in your UE level update automatically — no manual reimport needed.

## Setup
- Follow the [First Import](FirstImport.md) guide to import your scene.
- Add the imported asset to your level.
- Keep Unreal Engine open in the background.

> **No editor preference required.** Cygon Link ships with its own file watcher that monitors your project's `Content/` folder and triggers the reimport automatically. You do **not** need to enable Unreal Engine's built-in *Auto Reimport / Monitor Content Directories* setting — it works out of the box as long as the scene is exported inside `Content/` (see [First Import](FirstImport.md), Step 1).

## Live Editing Loop
Once set up, the workflow is:
- Open your scene in **Cygon**.
- Make changes — modify geometry or transforms.
- Export with **CTRL + S** or the Cygon export button.
- Switch back to **Unreal Engine**.
- Cygon Link detects the changed `.usda` file and runs its reimport pipeline automatically.
- All assets and their instances in the level update silently, without any manual step.
