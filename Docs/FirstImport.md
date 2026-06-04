# Your First Import

This guide walks you through importing a Cygon scene into Unreal Engine for the first time.

## Step 1 — Export from Cygon

In Cygon, export your scene directly into the `Content/` folder of your Unreal Engine project.

- Use **CTRL + S** or the export button in the Cygon UI.
- Cygon will generate a `.usda` file along with a `meshes/` subfolder containing individual mesh files.

**Example output structure inside your UE Content folder:**
```
Content/
└── MyScene/
    ├── MyScene.usda          ← top-level scene file
    └── meshes/
        ├── Cube.usda
        └── Floor.usda
```

> Exporting directly into the `Content/` folder is required. The plugin resolves asset paths relative to this location.

## Step 2 — Import into Unreal Engine

1. Switch to Unreal Engine. The Content Browser should detect the new files automatically.
2. If it does not appear, right-click in the Content Browser and select **Refresh**.
3. Locate the top-level `.usda` file (e.g. `MyScene.usda`) and **double-click** it to import, or drag it into a folder in the Content Browser.
4. Cygon Link will intercept the import and process it through the USD pipeline.
5. Native UE assets (Static Meshes, Materials, etc.) will be generated automatically in the same folder.

> Sub-mesh files inside the `meshes/` folder are intentionally skipped — do not import them individually.

## Step 3 — Add to Level

Drag the imported asset from the Content Browser into your **Level Viewport** or **Outliner**.

## Testing Without Cygon

A sample file is included at [`Docs/CygonSample.usda`](CygonSample.usda).

You can drag it directly into the Content Browser to verify that:
- Cygon Link correctly intercepts the file.
- The USD Importer processes it and generates assets.
- No errors appear in the Output Log.
