// Copyright 2026 Inspyr Studio, SAS. All Rights Reserved.

#include "CygonLink.h"
#include "CygonImportWatcher.h"

#define LOCTEXT_NAMESPACE "FCygonLinkModule"

FCygonLinkModule::FCygonLinkModule() = default;
FCygonLinkModule::~FCygonLinkModule() = default;

void FCygonLinkModule::StartupModule()
{
	ImportWatcher = MakeUnique<FCygonImportWatcher>();
	ImportWatcher->Start();
}

void FCygonLinkModule::ShutdownModule()
{
	if (ImportWatcher)
	{
		ImportWatcher->Stop();
		ImportWatcher.Reset();
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCygonLinkModule, CygonLink)
