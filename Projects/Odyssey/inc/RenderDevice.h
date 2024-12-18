//#pragma once
//#include "EngineIncludes.h"
//#include "RenderIncludes.h"
//#include "RenderTypes.h"
//#include "EngineEvents.h"
//
//namespace Odyssey
//{
//	class Application;
//	class Buffer;
//	class DepthState;
//	class BlendState;
//	class Sprite2DPass;
//	class Entity;
//	class ClearRenderTargetPass;
//	class Scene;
//	class SkyboxPass;
//	class ShadowPass;
//	class RenderTarget;
//	class OpaquePass;
//	class TransparentPass;
//	class RenderState;
//	class DebugPass;
//	class Light;
//	class Material;
//	class SamplerState;
//	class ScriptDataLoader;
//	class Shader;
//	class Texture;
//	class RenderWindow;
//	class Viewport;
//	class Mesh;
//
//	class RenderDevice : public std::enable_shared_from_this<RenderDevice>
//	{
//	public:
//		RenderDevice();
//		Microsoft::WRL::ComPtr<ID3D11Device> getDevice();
//		Microsoft::WRL::ComPtr<ID2D1DeviceContext> getDevice2DContext();
//		Microsoft::WRL::ComPtr<ID2D1Factory1> get2DFactory();
//	public:
//		std::shared_ptr<Scene> createScene();
//		std::shared_ptr<Scene> createScene(DirectX::XMFLOAT3 center, float radius);
//		void importScene(std::shared_ptr<Scene> scene, const char* filename);
//		void importModel(std::shared_ptr<Entity> entity, const char* filename, bool isMultiMesh);
//	public: // Rendering Resources
//		std::shared_ptr<Buffer> createBuffer(BufferBindFlag bindFlag, size_t size, UINT stride, const void* data);
//		std::shared_ptr<RenderState> createRenderState(Topology topology, CullMode cullMode, FillMode fillMode, bool frontCCW = false, bool depthClipping = true, bool isShadowMap = false);
//		std::shared_ptr<Material> createMaterial();
//		std::shared_ptr<Material> createMaterial(TextureType textureType, std::shared_ptr<Texture> texture);
//		std::shared_ptr<SamplerState> createSamplerState(ComparisonFunc comparisonFunc, D3D11_FILTER filter, int bindSlot);
//		std::shared_ptr<DepthState> createDepthState(bool depthWriteEnable, bool depthReadEnable, ComparisonFunc comparisonFunc);
//		std::shared_ptr<BlendState> createBlendState(BlendDesc& blendDesc, float blendFactor[4]);
//		std::shared_ptr<Shader> createShader(ShaderType shaderType, const char* filename, D3D11_INPUT_ELEMENT_DESC* layout, int numberOfElements = 0);
//		std::shared_ptr<Texture> createRenderTexture();
//		std::shared_ptr<Texture> createTexture(TextureType textureType, const char* filename);
//		std::shared_ptr<Viewport> createViewport(RenderWindow* renderWindow);
//		std::shared_ptr<Viewport> createViewport(int width, int height, int topLeftX, int topLeftY, float minDepth, float maxDepth);
//		std::shared_ptr<RenderTarget> createRenderTarget(int width, int height, bool depthEnabled);
//		std::shared_ptr<RenderTarget> createRenderTarget(int width, int height, bool depthEnabled, RenderWindow* renderWindow);
//		std::shared_ptr<Mesh> createMesh(std::vector<Vertex> vertexList, std::vector<unsigned int> indexList);
//		std::shared_ptr<Mesh> createCube(DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 center);
//	public: // Render Passes
//		std::shared_ptr<ClearRenderTargetPass> createClearRTVPass(std::shared_ptr<RenderWindow> renderWindow, bool clearDepth);
//		std::shared_ptr<SkyboxPass> createSkyboxPass(std::shared_ptr<RenderWindow> renderWindow);
//		std::shared_ptr<ShadowPass> createShadowPass(int texWidth, int texHeight);
//		std::shared_ptr<OpaquePass> createOpaquePass(std::shared_ptr<RenderWindow> renderWindow);
//		std::shared_ptr<TransparentPass> createTransparentPass(std::shared_ptr<RenderWindow> renderWindow);
//		std::shared_ptr<DebugPass> createDebugPass(std::shared_ptr<RenderWindow> renderWindow);
//		std::shared_ptr<Sprite2DPass> createSprite2DPass(std::shared_ptr<RenderWindow> renderWindow);
//	private:
//		void createDevice();
//		void findVideoCard();
//		// Direct3D
//		Microsoft::WRL::ComPtr<IDXGIAdapter> mVideoAdapter;
//		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
//		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
//		// Direct2D
//		Microsoft::WRL::ComPtr<ID2D1Factory1> mFactory;
//		Microsoft::WRL::ComPtr<ID2D1Device> mDevice2D;
//		Microsoft::WRL::ComPtr<ID2D1DeviceContext> mDevice2DContext;
//		std::shared_ptr<ScriptDataLoader> mScriptLoader;
//	private:
//		typedef std::vector<std::shared_ptr<Scene>> SceneList;
//		SceneList mScenes;
//	};
//}