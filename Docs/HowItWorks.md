# How It Works

## Overview

Cygon Link registers a custom **Asset Factory** (`UCygonUsdaFactory`) that intercepts `.usda` file imports inside Unreal Engine. When a `.usda` file is detected as a Cygon export, the factory delegates the actual asset conversion to the built-in **USD Importer** plugin, while applying Cygon-specific processing on top.

## File Detection

Not every `.usda` file is processed by Cygon Link — only files exported by Cygon.

The factory reads the **first line** of the `.usda` file. If it contains the word `Cygon`, the file is claimed for processing. Any other `.usda` file is left to the default UE USD pipeline.

**Example of a valid Cygon export header:**
```
#usda 1.0 - Exported by Cygon 0.2.0.7i
```

This ensures Cygon Link never interferes with `.usda` files coming from other tools (Houdini, Blender, etc.).

## Mesh Hierarchy Handling

Cygon exports scenes as a hierarchy of USDA files. Individual mesh files are placed inside a `meshes/` subfolder by Cygon automatically.

Cygon Link detects this and **skips sub-mesh files** during import, only processing the top-level scene file. This prevents duplicate assets from being created for each individual mesh.

## Import Pipeline

When a valid Cygon `.usda` file is imported:

1. Cygon Link creates an **Asset Import Task** targeting the correct destination folder in the Content Browser.
2. The task is handed off to `UsdStageAssetImportFactory` (from the built-in USD Importer plugin).
3. The USD Importer converts the USDA hierarchy into native UE assets: **Static Meshes**, **Materials**, **Textures**, etc.
4. All generated assets are saved to the Content Browser automatically.

## Reimport / Live Sync

Cygon Link also registers as a **Reimport Handler**. When a previously imported asset's source `.usda` file changes on disk (triggered by a Cygon export), UE's auto-reimport system detects the change and calls Cygon Link's reimport logic, which silently re-runs the full import pipeline and updates all instances in the level without any manual action.

See [LiveSync.md](LiveSync.md) for the full workflow.
