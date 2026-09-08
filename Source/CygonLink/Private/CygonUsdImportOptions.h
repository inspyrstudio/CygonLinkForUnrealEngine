// Copyright 2026 Inspyr Studio, SAS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/// Fixed values forced onto the native USD importer's options before every Cygon import, applied by UCygonUsdaFactory::ApplyDeterministicImportOptions.
namespace CygonUsdImportOptions
{
	struct FPinnedOption
	{
		const TCHAR* Name;
		const TCHAR* Value;
	};
	
	// Bitmask options are int32, so ImportText expects a plain number rather than flag names
	constexpr const TCHAR* AllPurposes = TEXT("7");           // EUsdPurpose: Default|Proxy|Render|Guide
	constexpr const TCHAR* CollapseAtAssembly = TEXT("30");   // EUsdDefaultKind: Component|Group|Assembly|Subcomponent
	constexpr const TCHAR* NaniteOff = TEXT("2147483647");    // MAX_int32 triangles required
	
	inline constexpr FPinnedOption PinnedValues[] = {
		// Cygon scenes carry geometry and materials only. Actors stay off: the components they spawned
		// tripped the SendAllEndOfFrameUpdates assertion.
		{TEXT("bImportActors"),                 TEXT("False")},
		{TEXT("bImportGeometry"),               TEXT("True")},
		{TEXT("bImportMaterials"),              TEXT("True")},
		{TEXT("bImportSkeletalAnimations"),     TEXT("False")},
		{TEXT("bImportLevelSequences"),         TEXT("False")},
		{TEXT("bImportGroomAssets"),            TEXT("False")},
		{TEXT("bImportSparseVolumeTextures"),   TEXT("False")},
		{TEXT("bImportSounds"),                 TEXT("False")},
		{TEXT("bImportOnlyUsedMaterials"),      TEXT("False")},
		
		// Never reuse whichever asset cache an artist happened to select.
		{TEXT("bUseExistingAssetCache"),        TEXT("False")},
		{TEXT("ExistingAssetCache"),            TEXT("None")},
		
		{TEXT("PurposesToImport"),              AllPurposes},
		{TEXT("NaniteTriangleThreshold"),       NaniteOff},
		{TEXT("RenderContextToImport"),         TEXT("unreal")},
		{TEXT("MaterialPurpose"),               TEXT("preview")},
		{TEXT("SubdivisionLevel"),              TEXT("0")},
		{TEXT("bOverrideStageOptions"),         TEXT("False")},
		{TEXT("bImportAtSpecificTimeCode"),     TEXT("False")},
		{TEXT("RootMotionHandling"),            TEXT("NoAdditionalRootMotion")},
		{TEXT("FallbackCollisionType"),         TEXT("None")},
		{TEXT("ExistingActorPolicy"),           TEXT("Replace")},
		{TEXT("ExistingAssetPolicy"),           TEXT("Replace")},
		{TEXT("bShareAssetsForIdenticalPrims"), TEXT("True")},
		
		// Collapsing stops at the topmost prim whose kind is listed. Cygon marks World as `assembly`,
		// so listing it collapses the scene into one SM_World that keeps the layout. 
		{TEXT("bPrimPathFolderStructure"),      TEXT("False")},
		{TEXT("bUsePrimKindsForCollapsing"),    TEXT("True")},
		{TEXT("KindsToCollapse"),               CollapseAtAssembly},
		
		{TEXT("bMergeIdenticalMaterialSlots"),  TEXT("True")},
		
		// Cygon never exports LOD variant sets, so this is a no-op on its scenes either way.
		{TEXT("bInterpretLODs"),                TEXT("False")},
	};
}
