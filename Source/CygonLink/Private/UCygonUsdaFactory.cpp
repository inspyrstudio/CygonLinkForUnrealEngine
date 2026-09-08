// Copyright 2026 Inspyr Studio, SAS. All Rights Reserved.

#include "UCygonUsdaFactory.h"
#include "CygonUsdImportOptions.h"
#include "Engine/StaticMesh.h"
#include "AssetImportTask.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "EditorFramework/AssetImportData.h" 
#include "PhysicsEngine/BodySetup.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/UObjectIterator.h"
#include "FileHelpers.h"
#include "Editor.h"
#include "TimerManager.h"
#include "Engine/CollisionProfile.h"
#include "HAL/FileManager.h"
#include "Misc/PackageName.h"
#include "Misc/StringOutputDevice.h"

UCygonUsdaFactory::UCygonUsdaFactory()
{
    bEditorImport = true;
    bText = false;
    SupportedClass = UStaticMesh::StaticClass();
    Formats.Add(TEXT("usda;Universal Scene Description ASCII"));
    
    ImportPriority = DefaultImportPriority + 1;
}

bool UCygonUsdaFactory::FactoryCanImport(const FString& Filename)
{
	if (!Filename.EndsWith(TEXT(".usda"))) return false;
	
	if (IsCygonFile(Filename)) return true;
	
	return false;
}

UObject* UCygonUsdaFactory::FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutCanceled)
{    
    if (IsSimpleMesh(Filename))
    {
        UE_LOG(LogTemp, Warning, TEXT("Sub mesh ignored : %s"), *Filename);
    	bOutCanceled = true; 
        
    	return nullptr; 
    }
	
	FString DestPath;
	if (!ResolveDestinationPath(Filename, DestPath))
	{
		bOutCanceled = true;
		return nullptr;
	}

	UAssetImportTask* ImportTask = CreateImportTask(Filename, DestPath);
	
	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	AssetToolsModule.Get().ImportAssetTasks({ ImportTask });
	
	const TArray<UObject*> ImportedObjects = ImportTask->GetObjects();
	if (ImportedObjects.Num() > 0)
	{
		MarkSceneImported(Filename);
		ApplyComplexAsSimpleCollision(ImportedObjects);
		
		return ImportedObjects[0];
	}
	
	return nullptr;
}

bool UCygonUsdaFactory::CanReimport(UObject* Obj, TArray<FString>& OutFilenames)
{
	FString SourceFilename = GetSourceFilename(Obj);
    
	if (SourceFilename.IsEmpty() || !SourceFilename.EndsWith(TEXT(".usda"))) return false;
	
	if (IsCygonFile(SourceFilename))
	{
		OutFilenames.Add(SourceFilename);
		return true;
	}
	
	return false;
}

void UCygonUsdaFactory::SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths)
{
	if (!Obj || NewReimportPaths.Num() == 0) return;
	
	if (UAssetImportData* ImportData = FindAssetImportData(Obj))
	{
		ImportData->UpdateFilenameOnly(NewReimportPaths[0]);
	}
}

bool UCygonUsdaFactory::bIsHandlingProxyCygonReimport = false;
TMap<FString, FString> UCygonUsdaFactory::ImportedSceneSignatures;

EReimportResult::Type UCygonUsdaFactory::Reimport(UObject* Obj)
{
	if (!Obj || bIsHandlingProxyCygonReimport) return EReimportResult::Failed;
	
	FString SourceFilename = GetSourceFilename(Obj);
	if (SourceFilename.IsEmpty()) return EReimportResult::Failed;
	if (IsSimpleMesh(SourceFilename)) return EReimportResult::Cancelled;
	
	// Every asset the scene produced records the scene as its source, so the reimport manager asks us to
	// reimport the whole stage once per asset. The first request does the work for all of them.
	if (IsSceneAlreadyImported(SourceFilename))
	{
		UE_LOG(LogTemp, Verbose, TEXT("[CygonLink] '%s' is unchanged since it was last imported; the reimport requested for '%s' is already covered."),
			*SourceFilename, *Obj->GetName());
		return EReimportResult::Succeeded;
	}
	
	FString DestPath;
	if (!ResolveDestinationPath(SourceFilename, DestPath)) return EReimportResult::Failed;
	
	TGuardValue<bool> ReentryGuard(bIsHandlingProxyCygonReimport, true);
	UE_LOG(LogTemp, Log, TEXT("[CygonLink] Reimporting Cygon scene '%s' into '%s' (requested for '%s')."), *SourceFilename, *DestPath, *Obj->GetName());
    
	UAssetImportTask* ImportTask = CreateImportTask(SourceFilename, DestPath);
	
	TGuardValue<bool> UnattendedGuard(GIsRunningUnattendedScript, true);

	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	AssetToolsModule.Get().ImportAssetTasks({ ImportTask });
	
	const TArray<UObject*> ImportedObjects = ImportTask->GetObjects();
	if (ImportedObjects.Num() > 0)
	{
		MarkSceneImported(SourceFilename);
		ApplyComplexAsSimpleCollision(ImportedObjects);
		
		UE_LOG(LogTemp, Log, TEXT("[CygonLink] Reimported %d asset(s) from '%s'."), ImportedObjects.Num(), *SourceFilename);
		return EReimportResult::Succeeded;
	}

	return EReimportResult::Failed;
}

bool UCygonUsdaFactory::IsCygonFile(const FString& Filename)
{
	FArchive* FileReader = IFileManager::Get().CreateFileReader(*Filename);
    
	if (!FileReader)
	{
		UE_LOG(LogTemp, Error, TEXT("Impossible to open the following file : %s"), *Filename);
		return false; 
	}
	
	FString FirstLine = TEXT("");
	uint8 SingleByte;
    
	while (!FileReader->AtEnd() && FirstLine.Len() < 100)
	{
		FileReader->Serialize(&SingleByte, 1);
        
		if (SingleByte == '\n' || SingleByte == '\r') break;
        
		FirstLine.AppendChar((TCHAR)SingleByte);
	}

	FileReader->Close();
	delete FileReader;
	
	if (FirstLine.Contains(TEXT("Cygon"))) return true; 
	
	return false;
}

bool UCygonUsdaFactory::IsSimpleMesh(const FString& Filename)
{
	FString NormalizedFilename = Filename;
	FPaths::NormalizeFilename(NormalizedFilename);
	FString DirectoryPath = FPaths::GetPath(NormalizedFilename);
	FString ParentFolderName = FPaths::GetPathLeaf(DirectoryPath);

	return ParentFolderName.Equals(TEXT("meshes"), ESearchCase::IgnoreCase);
}

UAssetImportData* UCygonUsdaFactory::FindAssetImportData(UObject* Obj)
{
	if (!Obj) return nullptr;
	
	if (FObjectProperty* Prop = FindFProperty<FObjectProperty>(Obj->GetClass(), "AssetImportData"))
	{
		return Cast<UAssetImportData>(Prop->GetObjectPropertyValue_InContainer(Obj));
	}
	
	if (UStaticMesh* StaticMesh = Cast<UStaticMesh>(Obj))
	{
		return StaticMesh->GetAssetImportData();
	}
	
	return nullptr;
}

FString UCygonUsdaFactory::GetSourceFilename(UObject* Obj) const
{
	if (UAssetImportData* ImportData = FindAssetImportData(Obj))
	{
		return ImportData->GetFirstFilename();
	}
	
	return TEXT("");
}

bool UCygonUsdaFactory::ResolveDestinationPath(const FString& SourceFilename, FString& OutDestinationPath)
{
	const FString FullFilename = FPaths::ConvertRelativePathToFull(SourceFilename);
	
	FString PackageName;
	if (!FPackageName::TryConvertFilenameToLongPackageName(FullFilename, PackageName))
	{
		UE_LOG(LogTemp, Warning, TEXT("[CygonLink] '%s' is not inside a mounted content folder; refusing to import it."), *FullFilename);
		return false;
	}
	
	OutDestinationPath = FPackageName::GetLongPackagePath(PackageName);
	
	// Assets belong to the project and nowhere else. Without this check a destination of `/Engine` writes straight into the engine installation's own Content folder
	if (OutDestinationPath != TEXT("/Game") && !OutDestinationPath.StartsWith(TEXT("/Game/")))
	{
		UE_LOG(LogTemp, Warning, TEXT("[CygonLink] '%s' resolves to '%s', which is outside the project's Content folder; refusing to import it."), *FullFilename, *OutDestinationPath);
		return false;
	}
	
	return true;
}

bool UCygonUsdaFactory::IsSceneAlreadyImported(const FString& SourceFilename)
{
	const FFileStatData Stat = IFileManager::Get().GetStatData(*SourceFilename);
	if (!Stat.bIsValid) return false;
	
	const FString Key = FPaths::ConvertRelativePathToFull(SourceFilename).ToLower();
	const FString* Recorded = ImportedSceneSignatures.Find(Key);
	
	return Recorded && *Recorded == FString::Printf(TEXT("%lld|%s"), Stat.FileSize, *Stat.ModificationTime.ToIso8601());
}

void UCygonUsdaFactory::MarkSceneImported(const FString& SourceFilename)
{
	const FFileStatData Stat = IFileManager::Get().GetStatData(*SourceFilename);
	if (!Stat.bIsValid) return;
	
	const FString Key = FPaths::ConvertRelativePathToFull(SourceFilename).ToLower();
	ImportedSceneSignatures.Add(Key, FString::Printf(TEXT("%lld|%s"), Stat.FileSize, *Stat.ModificationTime.ToIso8601()));
}

UAssetImportTask* UCygonUsdaFactory::CreateImportTask(const FString& Filename, const FString& DestinationPath)
{
	UAssetImportTask* ImportTask = NewObject<UAssetImportTask>();
	ImportTask->Filename = Filename; 
	ImportTask->DestinationPath = DestinationPath; 
	ImportTask->bAutomated = true;
	ImportTask->bReplaceExisting = true;
	ImportTask->bSave = !IsPlayInEditorActive();

	UClass* UsdFactoryClass = LoadObject<UClass>(nullptr, TEXT("/Script/USDStageImporter.UsdStageAssetImportFactory"));
	if (UsdFactoryClass)
	{
		ImportTask->Factory = NewObject<UFactory>(GetTransientPackage(), UsdFactoryClass);
	}
	
	UClass* UsdOptionsClass = LoadObject<UClass>(nullptr, TEXT("/Script/USDStageImporter.UsdStageImportOptions"));
	if (UsdOptionsClass)
	{
		UObject* ImportOptions = NewObject<UObject>(GetTransientPackage(), UsdOptionsClass);
		ApplyDeterministicImportOptions(ImportOptions);
		ImportTask->Options = ImportOptions;
	}
	return ImportTask;
}

void UCygonUsdaFactory::ApplyDeterministicImportOptions(UObject* ImportOptions)
{
	if (!ImportOptions) return;
	
	UClass* OptionsClass = ImportOptions->GetClass();
	
	// The values, and why they are forced at all, live in CygonUsdImportOptions.h.
	for (const CygonUsdImportOptions::FPinnedOption& Pinned : CygonUsdImportOptions::PinnedValues)
	{
		FProperty* Property = FindFProperty<FProperty>(OptionsClass, Pinned.Name);
		if (!Property)
		{
			UE_LOG(LogTemp, Warning,
				TEXT("[CygonLink] USD import option '%s' no longer exists on %s; this machine's EditorPerProjectUserSettings.ini value will be used instead."),
				Pinned.Name, *OptionsClass->GetName());
			continue;
		}
		
		FStringOutputDevice ImportErrors;
		if (Property->ImportText_InContainer(Pinned.Value, ImportOptions, ImportOptions, PPF_None, &ImportErrors) == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("[CygonLink] Could not pin USD import option '%s' to '%s': %s"), Pinned.Name, Pinned.Value, *ImportErrors);
		}
	}
}

bool UCygonUsdaFactory::IsPlayInEditorActive()
{
	return GEditor && GEditor->PlayWorld != nullptr;
}

TSet<TWeakObjectPtr<UPackage>> UCygonUsdaFactory::PiePendingSavePackages;
FDelegateHandle UCygonUsdaFactory::PieEndSaveHandle;

void UCygonUsdaFactory::SavePiePendingPackages(bool /*bIsSimulating*/)
{
	if (PieEndSaveHandle.IsValid())
	{
		FEditorDelegates::EndPIE.Remove(PieEndSaveHandle);
		PieEndSaveHandle.Reset();
	}
	
	TArray<UPackage*> Packages;
	for (const TWeakObjectPtr<UPackage>& WeakPackage : PiePendingSavePackages)
	{
		if (UPackage* Package = WeakPackage.Get())
		{
			Packages.Add(Package);
		}
	}
	PiePendingSavePackages.Empty();

	if (Packages.Num() > 0)
	{
		UEditorLoadingAndSavingUtils::SavePackages(Packages, true);
	}
}

void UCygonUsdaFactory::FinalizeImportedMeshCollision(const TArray<TWeakObjectPtr<UStaticMesh>>& Meshes)
{
	TArray<UPackage*> PackagesToSave;
	TSet<UStaticMesh*> RefreshedMeshes;
	RefreshedMeshes.Reserve(Meshes.Num());
	
	for (const TWeakObjectPtr<UStaticMesh>& WeakMesh : Meshes)
	{
		UStaticMesh* StaticMesh = WeakMesh.Get();
		if (!StaticMesh) continue;
		
		UBodySetup* BodySetup = StaticMesh->GetBodySetup();
		if (!BodySetup)
		{
			StaticMesh->CreateBodySetup();
			BodySetup = StaticMesh->GetBodySetup();
		}
		if (!BodySetup) continue;
		
		StaticMesh->Modify();
		BodySetup->Modify();
		BodySetup->RemoveSimpleCollision();
		BodySetup->bNeverNeedsCookedCollisionData = false;
		BodySetup->DefaultInstance.SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		BodySetup->DefaultInstance.SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
		BodySetup->CollisionTraceFlag = CTF_UseComplexAsSimple;
		BodySetup->InvalidatePhysicsData();
		BodySetup->CreatePhysicsMeshes();
		
		StaticMesh->CreateNavCollision(true);
		StaticMesh->MarkPackageDirty();
		
		RefreshedMeshes.Add(StaticMesh);
		PackagesToSave.AddUnique(StaticMesh->GetOutermost());
	}
	
	if (RefreshedMeshes.Num() == 0) return;
	
	if (IsPlayInEditorActive())
	{
		for (TObjectIterator<UStaticMeshComponent> CompIt; CompIt; ++CompIt)
		{
			if (RefreshedMeshes.Contains(CompIt->GetStaticMesh()))
			{
				CompIt->RecreatePhysicsState();
			}
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("[CygonLink] Enabled complex-as-simple collision on %d static mesh(es)."), RefreshedMeshes.Num());
	
	if (IsPlayInEditorActive())
	{
		for (UPackage* Package : PackagesToSave)
		{
			PiePendingSavePackages.Add(Package);
		}
		if (!PieEndSaveHandle.IsValid())
		{
			PieEndSaveHandle = FEditorDelegates::EndPIE.AddStatic(&UCygonUsdaFactory::SavePiePendingPackages);
		}
	}
	else
	{
		UEditorLoadingAndSavingUtils::SavePackages(PackagesToSave, true);
	}
}

void UCygonUsdaFactory::ApplyComplexAsSimpleCollision(const TArray<UObject*>& ImportedObjects) const
{
	TArray<TWeakObjectPtr<UStaticMesh>> meshes;
	for (UObject* Obj : ImportedObjects)
	{
		if (UStaticMesh* StaticMesh = Cast<UStaticMesh>(Obj))
		{
			meshes.Add(StaticMesh);
		}
	}
	
	if (meshes.Num() == 0 || !GEditor) return;
	
	GEditor->GetTimerManager()->SetTimerForNextTick([meshes]()
	{
		FinalizeImportedMeshCollision(meshes);
	});
}
