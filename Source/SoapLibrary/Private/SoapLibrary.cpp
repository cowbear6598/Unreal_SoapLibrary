// Copyright Epic Games, Inc. All Rights Reserved.

#include "SoapLibrary.h"

#define LOCTEXT_NAMESPACE "FSoapLibraryModule"

void FSoapLibraryModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FSoapLibraryModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSoapLibraryModule, SoapLibrary)