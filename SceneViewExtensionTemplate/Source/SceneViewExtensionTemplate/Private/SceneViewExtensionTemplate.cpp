// Custom SceneViewExtension Template for Unreal Engine
// Copyright 2023 - 2025 Ossi Luoto
// 
// Main Module to set Shader Directories

#include "SceneViewExtensionTemplate.h"

#define LOCTEXT_NAMESPACE "SceneViewExtensionTemplate"

void FSceneViewExtensionTemplate::StartupModule()
{
	// Set up the Shader Directories
	FString PluginShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("SceneViewExtensionTemplate"))->GetBaseDir(), TEXT("Shaders"));
	AddShaderSourceDirectoryMapping(TEXT("/Plugins/SceneViewExtensionTemplate"), PluginShaderDir);
}

void FSceneViewExtensionTemplate::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSceneViewExtensionTemplate, SceneViewExtensionTemplate);