// Custom SceneViewExtension Template for Unreal Engine
// Copyright 2023 Ossi Luoto
// 
// Custom SceneViewExtension implementation

#include "CustomSceneViewExtension.h"

IMPLEMENT_GLOBAL_SHADER(FCustomShader, "/Plugins/SceneViewExtensionTemplate/PostProcessCS.usf", "MainCS", SF_Compute);

namespace
{
	TAutoConsoleVariable<int32> CVarShaderOn(
		TEXT("r.SceneViewExtensionTemplate"),
		0,
		TEXT("Enable Custom SceneViewExtension \n")
		TEXT(" 0: OFF;")
		TEXT(" 1: ON."),
		ECVF_RenderThreadSafe);
}


FCustomSceneViewExtension::FCustomSceneViewExtension(const FAutoRegister& AutoRegister) : FSceneViewExtensionBase(AutoRegister)
{
	UE_LOG(LogTemp, Log, TEXT("SceneViewExtensionTemplate: Custom SceneViewExtension registered"));
}

void FCustomSceneViewExtension::SubscribeToPostProcessingPass(EPostProcessingPass PassId, FAfterPassCallbackDelegateArray& InOutPassCallbacks, bool bIsPassEnabled)
{
	// Define to what Post Processing stage to hook the SceneViewExtension into. See SceneViewExtension.h and PostProcessing.cpp for more info
	if (PassId == EPostProcessingPass::MotionBlur)
	{
		InOutPassCallbacks.Add(FAfterPassCallbackDelegate::CreateRaw(this, &FCustomSceneViewExtension::CustomPostProcessing));
	}
}

FScreenPassTexture FCustomSceneViewExtension::CustomPostProcessing(FRDGBuilder& GraphBuilder, const FSceneView& SceneView, const FPostProcessMaterialInputs& Inputs)
{
	// SceneViewExtension gives SceneView, not ViewInfo so we need to setup some basics
	const FSceneViewFamily& ViewFamily = *SceneView.Family;
	const ERHIFeatureLevel::Type FeatureLevel = SceneView.GetFeatureLevel();

	const FScreenPassTexture& SceneColor = Inputs.Textures[(uint32)EPostProcessMaterialInput::SceneColor];

	if (!SceneColor.IsValid() || CVarShaderOn.GetValueOnRenderThread() == 0)
	{
		return SceneColor;
	}

	// Here starts the RDG stuff
	RDG_EVENT_SCOPE(GraphBuilder, "Custom Postprocess Effect");
	{
		// Accesspoint to our Shaders
		FGlobalShaderMap* GlobalShaderMap = GetGlobalShaderMap(ViewFamily.GetFeatureLevel());

		// Setup all the descriptors to create a target texture
		FRDGTextureDesc OutputDesc;
		{
			OutputDesc = SceneColor.Texture->Desc;

			OutputDesc.Reset();
			OutputDesc.Flags |= TexCreate_UAV;
			OutputDesc.Flags &= ~(TexCreate_RenderTargetable | TexCreate_FastVRAM);

			FLinearColor ClearColor(0., 0., 0., 0.);
			OutputDesc.ClearValue = FClearValueBinding(ClearColor);
		}

		// Create target texture
		FRDGTextureRef OutputTexture = GraphBuilder.CreateTexture(OutputDesc, TEXT("Custom Effect Output Texture"));

		// Set the shader parameters
		FCustomShader::FParameters* PassParameters = GraphBuilder.AllocParameters<FCustomShader::FParameters>();

		// Input is the SceneColor from PostProcess Material Inputs
		PassParameters->OriginalSceneColor = SceneColor.Texture;

		// Get the input sizes (do note that viewport visible area might not be the full extent of the SceneColor texture
		// https://docs.unrealengine.com/5.1/en-US/screen-percentage-with-temporal-upscale-in-unreal-engine/
		FIntRect PassViewSize = SceneColor.ViewRect;
		FIntPoint SrcTextureSize = SceneColor.Texture->Desc.Extent;

		PassParameters->ViewportRect = PassViewSize;
		PassParameters->ViewportInvSize = FVector2f(1.0f / PassViewSize.Width(), 1.0f / PassViewSize.Height());

		// Conversion from the full texture to the actual used size
		// Refer to Screenpass.h to see how UE handles scaling of the different viewport sizes
		PassParameters->SceneColorUVScale = FVector2f(float(PassViewSize.Width()) / float(SrcTextureSize.X), float(PassViewSize.Height()) / float(SrcTextureSize.Y));

		// Method to setup common parameters, we use this to pass ViewUniformBuffer data
		// There is a lot of useful stuff in the ViewUniformBuffer, do note that when getting this from the SceneView, a lot them seem to be unpopulated
		FCommonShaderParameters CommonParameters;
		CommonParameters.ViewUniformBuffer = SceneView.ViewUniformBuffer;
		PassParameters->CommonParameters = CommonParameters;

		// Create UAV from Target Texture
		PassParameters->Output = GraphBuilder.CreateUAV(FRDGTextureUAVDesc(OutputTexture));

		// Set Compute Shader and execute
		const int32 kDefaultGroupSize = 8;
		FIntPoint GroupSize(kDefaultGroupSize, kDefaultGroupSize);
		FIntVector GroupCount = FComputeShaderUtils::GetGroupCount(PassViewSize.Size(), GroupSize);

		TShaderMapRef<FCustomShader> ComputeShader(GlobalShaderMap);

		FComputeShaderUtils::AddPass(
			GraphBuilder,
			RDG_EVENT_NAME("Custom SceneViewExtension Post Processing CS Shader %dx%d", PassViewSize.Width(), PassViewSize.Height()),
			ComputeShader,
			PassParameters,
			GroupCount);

		// Copy the output texture back to SceneColor
		// Returning the new texture as ScreenPassTexture doesn't work, so this is pretty fast alternative
		// Also with f.ex 'PrePostProcessPass_RenderThread' you get only input and something similar needs to be implemented then
		AddCopyTexturePass(GraphBuilder, OutputTexture, SceneColor.Texture);

	}

	// The call expects ScreenPassTexture as a return, we return with the same texture as we started with, see AddCopyTexturePass above 
	return SceneColor;
}
