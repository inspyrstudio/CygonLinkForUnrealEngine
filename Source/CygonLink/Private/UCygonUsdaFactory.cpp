// Copyright Inspyr Studio, SAS. All Rights Reserved.

#include "UCygonUsdaFactory.h"
#include "Engine/StaticMesh.h"
#include "AssetImportTask.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "EditorFramework/AssetImportData.h" 

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
    if (IsSimpleMeshe(CurrentFilename))
    {
        UE_LOG(LogTemp, Warning, TEXT("Sub mesh ignored : %s"), *CurrentFilename);
    	bOutCanceled = true; 
        
    	return nullptr; 
    }
	
	FString DestPath = InParent->GetPathName();
	FString UsdBaseName = FPaths::GetBaseFilename(Filename);
	
	if (FPaths::GetPathLeaf(DestPath).Equals(UsdBaseName, ESearchCase::IgnoreCase))
	{
		DestPath = FPaths::GetPath(DestPath);
	}

	UAssetImportTask* ImportTask = CreateImportTask(CurrentFilename, DestPath);
	
	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	AssetToolsModule.Get().ImportAssetTasks({ ImportTask });
	
	const TArray<UObject*> ImportedObjects = ImportTask->GetObjects();
	if (ImportedObjects.Num() > 0)
	{
		return ImportedObjects[0];
	}
	
	return nullptr;
}

bool UCygonUsdaFactory::CanReimport(UObject* Obj, TArray<FString>& OutFilenames)
{
	FString SourceFilename = GetSourceFilename(Obj);
    
	if (SourceFilename.IsEmpty() || !SourceFilename.EndsWith(TEXT(".usda"))) return false;
	
	if (IsCygonFile(SourceFilename)) return true;
	
	return false;
}

void UCygonUsdaFactory::SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths)
{
	if (!Obj) return;
	
	FObjectProperty* Prop = FindFProperty<FObjectProperty>(Obj->GetClass(), "AssetImportData");
	UStaticMesh* SM = Cast<UStaticMesh>(Obj);
	if (Prop)
	{
		if (UAssetImportData* ImportData = Cast<UAssetImportData>(Prop->GetObjectPropertyValue_InContainer(Obj)))
			ImportData->UpdateFilenameOnly(NewReimportPaths[0]);
	}
	else if (SM)
	{
		UAssetImportData* ImportData = SM->GetAssetImportData();
		if (ImportData)
			ImportData->UpdateFilenameOnly(NewReimportPaths[0]);
	}
}

static bool bIsHandlingProxyCygonReimport = false; // To avoid potential loops when reimporting an asset

EReimportResult::Type UCygonUsdaFactory::Reimport(UObject* Obj)
{
	if (!Obj || bIsHandlingProxyCygonReimport) return EReimportResult::Failed;
	
	FString SourceFilename = GetSourceFilename(Obj);
	if (SourceFilename.IsEmpty()) return EReimportResult::Failed;
	if (IsSimpleMeshe(SourceFilename)) return EReimportResult::Cancelled;
	
	bIsHandlingProxyCygonReimport = true;
	UE_LOG(LogTemp, Warning, TEXT("Asset update triggered on: %s"), *Obj->GetName());
	
	FString UsdBaseName = FPaths::GetBaseFilename(SourceFilename); 
    
	FString DestPath = FPaths::GetPath(Obj->GetPathName());
    
	FString SearchFolder = TEXT("/") + UsdBaseName;
	int32 FoundIndex = DestPath.Find(SearchFolder, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
    
	if (FoundIndex != INDEX_NONE)
	{
		DestPath = DestPath.Left(FoundIndex);
	}
	else
	{
		DestPath = FPaths::GetPath(Obj->GetPathName());
	}
	UAssetImportTask* ImportTask = CreateImportTask(SourceFilename, DestPath);
	
	bool bWasUnattended = GIsRunningUnattendedScript;
	GIsRunningUnattendedScript = true;

	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	AssetToolsModule.Get().ImportAssetTasks({ ImportTask });

	GIsRunningUnattendedScript = bWasUnattended;
	bIsHandlingProxyCygonReimport = false;
	
	if (ImportTask->GetObjects().Num() > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Asset reimported successfully: %s"), *Obj->GetName());
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

bool UCygonUsdaFactory::IsSimpleMeshe(const FString& Filename)
{
	FString NormalizedFilename = Filename;
	FPaths::NormalizeFilename(NormalizedFilename);
	FString DirectoryPath = FPaths::GetPath(NormalizedFilename);
	FString ParentFolderName = FPaths::GetPathLeaf(DirectoryPath);

	return ParentFolderName.Equals(TEXT("meshes"), ESearchCase::IgnoreCase);
}

FString UCygonUsdaFactory::GetSourceFilename(UObject* Obj) const
{
	if (!Obj) return TEXT("");
	
	FObjectProperty* Prop = FindFProperty<FObjectProperty>(Obj->GetClass(), "AssetImportData");
	if (Prop)
	{
		if (UAssetImportData* ImportData = Cast<UAssetImportData>(Prop->GetObjectPropertyValue_InContainer(Obj)))
			return ImportData->GetFirstFilename();
	}
	else if (UStaticMesh* SM = Cast<UStaticMesh>(Obj))
	{
		if (SM->GetAssetImportData())
			return SM->GetAssetImportData()->GetFirstFilename();
	}
	return TEXT("");
}

UAssetImportTask* UCygonUsdaFactory::CreateImportTask(const FString& Filename, const FString& DestinationPath)
{
	UAssetImportTask* ImportTask = NewObject<UAssetImportTask>();
	ImportTask->Filename = Filename; 
	ImportTask->DestinationPath = DestinationPath; 
	ImportTask->bAutomated = true;
	ImportTask->bReplaceExisting = true;
	ImportTask->bSave = true;
	
	UClass* UsdFactoryClass = LoadObject<UClass>(nullptr, TEXT("/Script/USDStageImporter.UsdStageAssetImportFactory"));
	if (UsdFactoryClass)
	{
		ImportTask->Factory = NewObject<UFactory>(GetTransientPackage(), UsdFactoryClass);
	}

	UClass* UsdOptionsClass = LoadObject<UClass>(nullptr, TEXT("/Script/USDStageImporter.UsdStageImportOptions"));
	if (UsdOptionsClass)
	{
		UObject* ImportOptions = NewObject<UObject>(GetTransientPackage(), UsdOptionsClass);
		
		ImportTask->Options = ImportOptions;
	}
	return ImportTask;
}
