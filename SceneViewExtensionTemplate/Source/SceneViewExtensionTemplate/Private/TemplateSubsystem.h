// Custom SceneViewExtension Template for Unreal Engine
// Copyright 2023 - 2025 Ossi Luoto
// 
// Subsystem to keep custom SceneViewExtension alive

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "TemplateSubsystem.generated.h"

 UCLASS()
class UTemplateSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	TSharedPtr<class FCustomSceneViewExtension, ESPMode::ThreadSafe> CustomSceneViewExtension;
};