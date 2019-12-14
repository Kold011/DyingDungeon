#include "ShadowPass.h"
#include "RenderTarget.h"
#include "Light.h"
#include "RenderState.h"
#include "Buffer.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Transform.h"
#include "RenderDevice.h"
#include "Shader.h"
#include "Frustum.h"
#include "AABB.h"
#include "Camera.h"
#include "AnimatorDX11.h"

namespace Odyssey
{
	ShadowPass::ShadowPass(RenderDevice& renderDevice, std::shared_ptr<Light> shadowLight, int texWidth, int texHeight)
	{
		// Get the device and context
		mDevice = renderDevice.getDevice();
		mDevice->GetImmediateContext(mDeviceContext.GetAddressOf());

		// Set the properties of the shadow pass
		mShadowLight = shadowLight;

		// Create the shadow map render target
		mRenderTarget = renderDevice.createRenderTarget(texWidth, texHeight, false);
		mRenderTarget->createDepthTarget(D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL, texWidth, texHeight);
		mRenderTarget->getDepthTexture()->createSRV(DXGI_FORMAT_R24_UNORM_X8_TYPELESS);

		// Create the shadow map viewport
		std::shared_ptr<Viewport> shadowViewport = renderDevice.createViewport(texWidth, texHeight, 0, 0, 0.0f, 1.0f);
		mRenderTarget->setViewport(shadowViewport);

		// Create the render state
		mRenderState = renderDevice.createRenderState(Topology::TriangleList, CullMode::CULL_BACK, FillMode::FILL_SOLID, false, true, true);

		// Create a shadow buffer to tell the pixel shader a shadow map is bound
		DirectX::XMFLOAT4 shadowsEnabled = { 1.0f, 1.0f, 1.0f, 1.0f };
		mShadowBuffer = renderDevice.createBuffer(BufferBindFlag::ConstantBuffer, size_t(1),
			static_cast<UINT>(sizeof(DirectX::XMFLOAT4)), &shadowsEnabled);
		mShadowBuffer->bind(2, ShaderType::PixelShader);

		// Create the vertex shader
		D3D11_INPUT_ELEMENT_DESC vShaderLayout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, 64, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 80, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		// Create the depth vertex and pixel shaders
		mVertexShader = renderDevice.createShader(ShaderType::VertexShader, "../OdysseyGameEngine/shaders/DepthVertexShader.cso", vShaderLayout, 7);
		mPixelShader = renderDevice.createShader(ShaderType::PixelShader, "../OdysseyGameEngine/shaders/DepthPixelShader.cso", nullptr, 0);
	}

	void ShadowPass::preRender(RenderArgs& args)
	{

		// Recalculate the light's transform and set the shader matrix's lightViewProj
		float sceneRadius = 50.0f;
		DirectX::XMFLOAT3 sceneCenter = { 0.0f, 0.0f, 0.0f };
		DirectX::XMFLOAT4X4 lightTransform = mShadowLight->buildLightTransform(sceneRadius, sceneCenter);
		DirectX::XMStoreFloat4x4(&args.perFrame.view, DirectX::XMLoadFloat4x4(&lightTransform));

		lightTransform = mShadowLight->buildLightTransformProjection(sceneRadius, sceneCenter);
		DirectX::XMStoreFloat4x4(&args.perFrame.lightViewProj, DirectX::XMLoadFloat4x4(&lightTransform));

		// Update the per frame buffer
		updatePerFrameBuffer(args.perFrame, args.perFrameBuffer);

		// Unbind the depth texture from the pipeline
		mRenderTarget->unbindDepthTexture();

		// Clear the depth texture of the render target
		mRenderTarget->clearDepth();

		// Bind the render target to the pipeline
		mRenderTarget->bind();

		// Bind the render state to the pipeline
		mRenderState->bind();

		// Bind the shaders
		mVertexShader->bind();
		mPixelShader->unbind();
	}

	void ShadowPass::render(RenderArgs& args)
	{
		std::multimap<float, std::shared_ptr<GameObject>> renderMap;

		DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4(&args.perFrame.view);

		Microsoft::WRL::ComPtr<ID3DUserDefinedAnnotation> performance;
		mDeviceContext->QueryInterface(__uuidof(performance), &performance);
		performance->BeginEvent(L"Shadow Pass");

		// Iterate over each scene object in the render list
		for (std::shared_ptr<GameObject> renderObject : args.renderList)
		{
			if (AnimatorDX11* rootAnimator = renderObject->getRootComponent<AnimatorDX11>())
			{
				rootAnimator->bind();
			}

			if (MeshRenderer* meshRenderer = renderObject->getComponent<MeshRenderer>())
			{
				if (meshRenderer->getActive())
				{
					renderSceneObject(renderObject, args);
				}
			}

			for (std::shared_ptr<GameObject> child : renderObject->getChildren())
			{
				if (MeshRenderer* meshRenderer = child->getComponent<MeshRenderer>())
				{
					if (meshRenderer->getActive())
					{
						renderSceneObject(child, args);
					}
				}
			}
			if (AnimatorDX11* rootAnimator = renderObject->getRootComponent<AnimatorDX11>())
			{
				rootAnimator->unbind();
			}
		}

		performance->EndEvent();

		// Unbind the render target from the pipeline
		mRenderTarget->unBind();

		// Bind the depth texture to the pixel shader pipeline
		mRenderTarget->bindDepthTexture();
	}

	void ShadowPass::renderSceneObject(std::shared_ptr<GameObject> object, RenderArgs& args)
	{
		// Get the object's global transform and set the MVP acoordingly
		object->getComponent<Transform>()->getGlobalTransform(args.perObject.world);

		// Update and bind the constant buffer
		updatePerObjectBuffer(args.perObject, args.perObjectBuffer);

		// Bind the vertex and index buffer of the mesh to the pipeline
		object->getComponent<MeshRenderer>()->getMesh()->getIndexBuffer()->bind();
		object->getComponent<MeshRenderer>()->getMesh()->getVertexBuffer()->bind();

		// Draw the mesh
		mDeviceContext->DrawIndexed(object->getComponent<MeshRenderer>()->getMesh()->getNumberOfIndices(), 0, 0);
	}
}