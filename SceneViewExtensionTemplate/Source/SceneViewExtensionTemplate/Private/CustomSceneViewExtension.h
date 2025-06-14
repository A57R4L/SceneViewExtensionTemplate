// Custom SceneViewExtension Template for Unreal Engine
// Copyright 2023 - 2025 Ossi Luoto
// 
// Custom SceneViewExtension implementation

#pragma once

#include "CoreMinimal.h"
#include "RenderGraphUtils.h"
#include "SceneViewExtension.h"
#include "PostProcess/PostProcessMaterial.h"
#include "DataDrivenShaderPlatformInfo.h"

class FCustomSceneViewExtension : public FSceneViewExtensionBase
{
public:
	FCustomSceneViewExtension(const FAutoRegister& AutoRegister);

public:
	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override {};
	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override {};
	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override {};

	// See SceneViewExtension.h for hooks to different stages of rendering
	// f.ex. PrePostProcessPass_RenderThread happens just when rendering is finished but PostProcessing hasn't started yet

	// This is the method to hook into PostProcessing pass
	virtual void SubscribeToPostProcessingPass(EPostProcessingPass PassId, const FSceneView& View, FAfterPassCallbackDelegateArray& InOutPassCallbacks, bool bIsPassEnabled);

	// This is our actual processing function
	FScreenPassTexture CustomPostProcessing(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessMaterialInputs& Inputs);

};


// Custom Post Process Shader
class SCENEVIEWEXTENSIONTEMPLATE_API FCustomShader : public FGlobalShader
{
public:
	DECLARE_GLOBAL_SHADER(FCustomShader)

	SHADER_USE_PARAMETER_STRUCT(FCustomShader, FGlobalShader)

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER_STRUCT(FScreenPassTextureViewportParameters, SceneColorViewport)
		SHADER_PARAMETER_RDG_TEXTURE(Texture2D, OriginalSceneColor)
		SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<float4>, Output)
	END_SHADER_PARAMETER_STRUCT()

	// Basic shader initialization
	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
	}

	// Define environment variables used by compute shader
	static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
	{
		OutEnvironment.SetDefine(TEXT("THREADS_X"), 8);
		OutEnvironment.SetDefine(TEXT("THREADS_Y"), 8);
		OutEnvironment.SetDefine(TEXT("THREADS_Z"), 1);
	}
};


