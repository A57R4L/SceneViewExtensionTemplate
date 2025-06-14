// Custom SceneViewExtension Template for Unreal Engine
// Copyright 2023 - 2025 Ossi Luoto
// 
// Subsystem to keep custom SceneViewExtension alive

#include "TemplateSubsystem.h"
#include "CustomSceneViewExtension.h"
#include "SceneViewExtension.h"

void UTemplateSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	CustomSceneViewExtension = FSceneViewExtensions::NewExtension<FCustomSceneViewExtension>();
	UE_LOG(LogTemp, Log, TEXT("SceneViewExtensionTemplate: Subsystem initialized & SceneViewExtension created"));
}

void UTemplateSubsystem::Deinitialize()
{
	{
		CustomSceneViewExtension->IsActiveThisFrameFunctions.Empty();

		FSceneViewExtensionIsActiveFunctor IsActiveFunctor;

		IsActiveFunctor.IsActiveFunction = [](const ISceneViewExtension* SceneViewExtension, const FSceneViewExtensionContext& Context)
		{
			return TOptional<bool>(false);
		};

		CustomSceneViewExtension->IsActiveThisFrameFunctions.Add(IsActiveFunctor);
	}

	CustomSceneViewExtension.Reset();
	CustomSceneViewExtension = nullptr;
}
