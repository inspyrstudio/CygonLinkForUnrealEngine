// Copyright Inspyr Studio, SAS. All Rights Reserved.

#include "CygonImportWatcher.h"
#include "UCygonUsdaFactory.h"
#include "AssetImportTask.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "DirectoryWatcherModule.h"
#include "IDirectoryWatcher.h"
#include "Modules/ModuleManager.h"
#include "Misc/Paths.h"
#include "Misc/PackageName.h"
#include "HAL/PlatformTime.h"

FCygonImportWatcher::~FCygonImportWatcher()
{
	Stop();
}

void FCygonImportWatcher::Start()
{
	if (DirectoryWatcherHandle.IsValid()) return; 
	
	FDirectoryWatcherModule& Module = FModuleManager::LoadModuleChecked<FDirectoryWatcherModule>(DirectoryWatcherModuleName);
	IDirectoryWatcher* DirectoryWatcher = Module.Get();
	if (!DirectoryWatcher)
	{
		UE_LOG(LogTemp, Warning, TEXT("[CygonLink] DirectoryWatcher unavailable; automatic import disabled."));
		return;
	}
	
	WatchedDirectory = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir());
	
	DirectoryWatcher->RegisterDirectoryChangedCallback_Handle(WatchedDirectory, IDirectoryWatcher::FDirectoryChanged::CreateRaw(this, &FCygonImportWatcher::OnDirectoryChanged), DirectoryWatcherHandle);
	
	TickerHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateRaw(this, &FCygonImportWatcher::Tick), 0.5f);

	UE_LOG(LogTemp, Log, TEXT("[CygonLink] Watching for Cygon .usda exports in: %s"), *WatchedDirectory);
}

void FCygonImportWatcher::Stop()
{
	if (TickerHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(TickerHandle);
		TickerHandle.Reset();
	}
	
	if (DirectoryWatcherHandle.IsValid() && FModuleManager::Get().IsModuleLoaded(DirectoryWatcherModuleName))
	{
		FDirectoryWatcherModule& Module = FModuleManager::GetModuleChecked<FDirectoryWatcherModule>(DirectoryWatcherModuleName);
		if (IDirectoryWatcher* DirectoryWatcher = Module.Get())
		{
			DirectoryWatcher->UnregisterDirectoryChangedCallback_Handle(WatchedDirectory, DirectoryWatcherHandle);
		}
	}
	
	DirectoryWatcherHandle.Reset();
	PendingScenes.Empty();
}

void FCygonImportWatcher::OnDirectoryChanged(const TArray<FFileChangeData>& FileChanges)
{
	bool bRelevantChange = false;
	
	for (const FFileChangeData& Change : FileChanges)
	{
		if (Change.Action == FFileChangeData::FCA_Removed) continue;
		
		if (!Change.Filename.EndsWith(TEXT(".usda"))) continue;
		
		bRelevantChange = true;
		
		if (!UCygonUsdaFactory::IsSimpleMesh(Change.Filename))
		{
			PendingScenes.Add(FPaths::ConvertRelativePathToFull(Change.Filename));
		}
	}
	
	if (bRelevantChange)
	{
		LastChangeTimeSeconds = FPlatformTime::Seconds();
	}
}

bool FCygonImportWatcher::Tick(float DeltaTime)
{
	if (PendingScenes.Num() == 0) return true;
	
	if (FPlatformTime::Seconds() - LastChangeTimeSeconds < ImportDebounceSeconds) return true;
	
	auto It = PendingScenes.CreateIterator();
	const FString Scene = *It;
	It.RemoveCurrent();
	
	TriggerImport(Scene);
	
	return true;
}

void FCygonImportWatcher::TriggerImport(const FString& AbsoluteFilename)
{
	if (!FPaths::FileExists(AbsoluteFilename)) return;
	
	if (!UCygonUsdaFactory::IsCygonFile(AbsoluteFilename)) return;
	
	FString PackageName;
	if (!FPackageName::TryConvertFilenameToLongPackageName(AbsoluteFilename, PackageName))
	{
		UE_LOG(LogTemp, Warning, TEXT("[CygonLink] Ignoring Cygon file outside Content/: %s"), *AbsoluteFilename);
		return;
	}
	const FString DestinationPath = FPackageName::GetLongPackagePath(PackageName);
	
	UAssetImportTask* ImportTask = NewObject<UAssetImportTask>();
	ImportTask->Filename = AbsoluteFilename;
	ImportTask->DestinationPath = DestinationPath;
	ImportTask->bAutomated = true;
	ImportTask->bReplaceExisting = true;
	ImportTask->bSave = !UCygonUsdaFactory::IsPlayInEditorActive();
	ImportTask->Factory = NewObject<UCygonUsdaFactory>();
	
	TGuardValue<bool> UnattendedGuard(GIsRunningUnattendedScript, true);
	
	UE_LOG(LogTemp, Log, TEXT("[CygonLink] Auto-importing Cygon scene: %s -> %s"), *AbsoluteFilename, *DestinationPath);
	
	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));
	AssetToolsModule.Get().ImportAssetTasks({ ImportTask });
}
