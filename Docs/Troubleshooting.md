# Troubleshooting

## Plugin does not appear in Edit → Plugins

**Cause:** The plugin was not installed correctly or was not built.

**Fix:**
- If installed via Fab, confirm it was installed to the correct engine version in the Epic Games Launcher.
- If installed from source, right-click the `.uproject` and select **Generate Visual Studio project files**, then rebuild.

---

## `.usda` file is imported but no assets are generated

**Cause 1:** The USD Importer plugin is not enabled.

**Fix:** Go to **Edit → Plugins**, search for **USD Importer**, enable it, and restart the editor. See [Prerequisites](Prerequisites.md).

**Cause 2:** The `.usda` file was not exported by Cygon (missing `Cygon` marker in the first line).

**Fix:** Confirm the file was exported from Cygon 0.2.0.7i or later. Open the file in a text editor and verify the first line contains the word `Cygon`.

---

## Sub-mesh files are ignored / not imported

This is **expected behavior**. Cygon Link intentionally skips files located inside a `meshes/` subfolder to avoid creating duplicate assets. Only the top-level scene `.usda` file should be imported.

---

## Live sync is not triggering

**Cause:** Auto-reimport is not configured in UE.

**Fix:**
1. Go to **Edit → Editor Preferences → Auto Reimport**.
2. Enable **Monitor Content Directories**.
3. Confirm your project's `Content/` folder is listed.

---

## Import fails with errors in the Output Log

1. Open the **Output Log** (**Window → Output Log**).
2. Filter by `LogTemp` or `LogUsd` to see relevant messages.
3. Common causes:
   - The source `.usda` file path contains special characters — use simple ASCII paths.
   - The `Content/` folder path is too deeply nested — keep the export path short.

---

## Assets are duplicated after reimport

**Cause:** The asset was imported into the wrong destination folder, causing Cygon Link to create a second copy instead of updating the existing one.

**Fix:** Ensure you always export from Cygon directly into the `Content/` folder of your UE project, as described in [First Import](FirstImport.md).

---

## Still having issues?

Join the [Inspyr Studio Discord](https://inspyrstudio.com/) and report the problem, including your Output Log output and the Cygon and UE versions you are using.
