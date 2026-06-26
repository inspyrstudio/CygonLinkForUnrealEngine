// Copyright 2026 Inspyr Studio, SAS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Ticker.h"

struct FFileChangeData;

/// Watches the project's `Content/` directory for Cygon `.usda` scene files written directly to
/// disk and triggers their import automatically.
class FCygonImportWatcher
{
public:
	FCygonImportWatcher() = default;
	~FCygonImportWatcher();

	void Start();
	void Stop();

private:
	/// Engine callback fired whenever files change anywhere under the watched tree.
	void OnDirectoryChanged(const TArray<FFileChangeData>& FileChanges);

	/// Debounce pump: imports pending scenes once the export has settled on disk.
	bool Tick(float DeltaTime);

	/// Build and run an import task for a single Cygon scene file, reusing UCygonUsdaFactory.
	void TriggerImport(const FString& AbsoluteFilename);

	/// Absolute path of the watched directory (the project's Content folder).
	FString WatchedDirectory;

	/// Handle returned when registering the directory-changed callback.
	FDelegateHandle DirectoryWatcherHandle;

	/// Handle for the debounce ticker.
	FTSTicker::FDelegateHandle TickerHandle;

	/// Top-level scene files awaiting import. Sub-mesh files are never added here.
	TSet<FString> PendingScenes;

	double LastChangeTimeSeconds = 0.0;
	static constexpr double ImportDebounceSeconds = 2.0;

	/// Name of the engine DirectoryWatcher module this watcher binds to.
	static constexpr const TCHAR* DirectoryWatcherModuleName = TEXT("DirectoryWatcher");
};
