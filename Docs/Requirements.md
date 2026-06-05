# Requirements
Before using Cygon Link, make sure the following requirements are met.

## Software Requirements
| Software      | Minimum Version | Notes                                                        |
|---------------|-----------------|--------------------------------------------------------------|
| Unreal Engine | 5.7+            | Tested on UE 5.7+                                            |
| Cygon         | 0.2.3i+         | Earlier versions do not support the required export pipeline |

> **New to Cygon?** Watch the [Cygon installation tutorial on YouTube](https://www.youtube.com/watch?v=SaS8J_4AumM) to get up and running. And you can go see [what is cygon](WhatIsCygon.md) for a quick overview of the product.

## Required Unreal Engine Plugin
Cygon Link relies on the **USD Importer** plugin, which ships with Unreal Engine but may be disabled by default.

To enable it:
- Open your Unreal Engine project.
- Go to **Edit → Plugins**.
- Search for **USD Importer**.
- Check the box to enable it.
- Restart the editor when prompted.

> **Note:** Without the USD Importer plugin enabled, Cygon Link will not be able to process `.usda` files.

## Platform Support

| Platform       | Supported |
|----------------|-----------|
| Windows 64-bit | ✅        |
| macOS          | ✅        |
| Linux          | ❌        |
