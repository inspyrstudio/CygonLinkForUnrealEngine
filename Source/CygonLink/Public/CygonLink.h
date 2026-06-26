// Copyright 2026 Inspyr Studio, SAS. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "Templates/UniquePtr.h"

class FCygonImportWatcher;

class FCygonLinkModule : public IModuleInterface
{
public:
	FCygonLinkModule();
	virtual ~FCygonLinkModule() override;

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TUniquePtr<FCygonImportWatcher> ImportWatcher;
};
