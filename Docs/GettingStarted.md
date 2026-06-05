# Getting Started with Cygon Link
Cygon Link is a bridge between **Cygon** and **Unreal Engine**. [Cygon](WhatIsCygon.md) is a standalone 3D environment blockout and prototyping tool — Cygon Link lets you bring your blockouts into UE and see changes reflected instantly, eliminating the traditional export/import friction.

## What does it do?
- **Automated Asset Generation** — Converts USDA scene hierarchies exported by Cygon directly into native UE assets: Static Meshes, Materials, and more.
- **Live Hot-Reloading** — Detects file changes on disk and automatically reimports and updates all instances in your active level.
- **Intelligent Mesh Processing** — Uses UE's built-in USD Importer under the hood while applying Cygon-specific processing to ensure correct results.

## Before you start
Make sure you have the following ready:

| Software      | Minimum Version |
|---------------|-----------------|
| Unreal Engine | 5.7+            |
| Cygon         | 0.2.3i+         |

> New to Cygon? See [What is Cygon?](WhatIsCygon.md) and watch the [installation tutorial on YouTube](https://www.youtube.com/watch?v=SaS8J_4AumM).

The **USD Importer** plugin (built into UE) must also be enabled. See [Requirements](Requirements.md) for details.

## Step-by-step guides
Follow these in order for your first setup:

1. [Requirements](Requirements.md) — Check software versions and enable USD Importer
2. [Installation](Installation.md) — Install Cygon Link via Fab or from source
3. [First Import](FirstImport.md) — Import your first Cygon scene into UE
4. [Live Sync](LiveSync.md) — Set up real-time sync between Cygon and UE

## How it works
- Export your scene from Cygon into the `Content/` folder of your UE project with **CTRL + S**.
- Switch to Unreal Engine — assets are generated automatically in the Content Browser.
- Drag the asset into your level.
- Make changes in Cygon, export again — UE updates instantly.

For a deeper explanation of the pipeline, see [How It Works](HowItWorks.md).

## Dependencies
This plugin has no third-party code or libraries. The only dependency is the **USD Importer** plugin, which is an Epic Games built-in plugin shipped with Unreal Engine itself.
