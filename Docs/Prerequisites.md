# Prerequisites

Before using Cygon Link, make sure the following requirements are met.

## Software Requirements

| Software | Minimum Version | Notes |
|---|---|---|
| Unreal Engine | 5.7 | Tested on UE 5.7+ |
| Cygon | 0.2.0.7i | Earlier versions do not support the required export pipeline |

## Required Unreal Engine Plugin

Cygon Link relies on the **USD Importer** plugin, which ships with Unreal Engine but may be disabled by default.

To enable it:
1. Open your Unreal Engine project.
2. Go to **Edit → Plugins**.
3. Search for **USD Importer**.
4. Check the box to enable it.
5. Restart the editor when prompted.

> **Note:** Without the USD Importer plugin enabled, Cygon Link will not be able to process `.usda` files.

## Platform Support

| Platform | Supported |
|---|---|
| Windows 64-bit | ✅ |
| macOS | ✅ |
| Linux | ❌ |
