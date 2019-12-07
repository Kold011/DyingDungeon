#include "Scene.h"
#include "BufferManager.h"
#include "TextureManager.h"
#include "Material.h"
#include "MaterialManager.h"
#include "MeshManager.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "SceneObject.h"
#include "RenderPass.h"
#include "RenderPipelineManager.h"
#include "ParticleSystem.h"
#include "Buffer.h"
#include "Component.h"

namespace Odyssey
{
	Scene::Scene()
	{
		mLightingBuffer = BufferManager::getInstance().createBuffer(BufferBindFlag::ConstantBuffer, 1, sizeof(SceneLighting), nullptr);
		mShaderMatrixBuffer = BufferManager::getInstance().createBuffer(BufferBindFlag::ConstantBuffer, 1, sizeof(ShaderMatrix), nullptr);
	}

	void Scene::addLight(std::shared_ptr<Light> light)
	{
		mSceneLights.push_back(light);
	}

	void Scene::addSceneObject(std::shared_ptr<SceneObject> sceneObject)
	{
		mSceneObjectList.push_back(sceneObject);
	}

	void Scene::initialize()
	{
		mXTimer.Restart();
		for (std::shared_ptr<SceneObject> sceneObject : mSceneObjectList)
		{
			for (auto* component : sceneObject->getComponents<Component>())
			{
				component->initialize(sceneObject.get());
			}
		}

		// Update the render args lists
		renderArgs.shaderMatrixBuffer = mShaderMatrixBuffer;
		renderArgs.camera = &mMainCamera;
		renderArgs.lightList = mSceneLights;
		renderArgs.renderList = mSceneObjectList;
	}

	void Scene::update()
	{
		mXTimer.Signal();

		// Recalculate delta time
		mDeltaTime = mXTimer.SmoothDelta();

		// Updates the lighting buffer
		updateLightingBuffer();

		// Update the scene
		updateScene();

		// Render the scene
		RenderPipelineManager::getInstance().render(renderArgs);
	}

	double Scene::getDeltaTime()
	{
		return mDeltaTime;
	}

	void Scene::updateScene()
	{
		// Generate a render list
		for (std::shared_ptr<SceneObject> renderObject : mSceneObjectList)
		{
			for (auto& component : renderObject->getComponents<Component>())
			{
				component->update(mDeltaTime);
			}

			//if (renderObject->hasParticleSystem())
			//{
			//	args.transparentList.push_back(renderObject);
			//}

			//// If the object has a mesh renderer push it, and its children, onto the render list
			//if (renderObject->hasMeshRenderer())
			//{
			//	args.renderList.push_back(renderObject);
			//}
			//else
			//{
			//	// If the parent object doesn't have a mesh renderer, search the children for a mesh renderer
			//	for (std::shared_ptr<SceneObject> child : renderObject->getChildren())
			//	{
			//		// The child has a mesh renderer, push it's parent, and the parent's children onto the render list
			//		if (child->hasMeshRenderer())
			//		{
			//			args.renderList.push_back(renderObject);
			//			break;
			//		}
			//	}
			//}
		}

	}

	void Scene::updateLightingBuffer()
	{
		// Generate a list of lights on a per-object basis
		SceneLighting sceneLighting;
		sceneLighting.numLights = 0;

		// Set the camera's position for specular highlighting
		sceneLighting.camPos = DirectX::XMFLOAT3(mMainCamera.getViewMatrix().m[3][0], mMainCamera.getViewMatrix().m[3][1], mMainCamera.getViewMatrix().m[3][2]);

		// Iterate through the entire scene light list
		for (int i = 0; i < mSceneLights.size(); i++)
		{
			// Directional and spot lights are automatically added to the light list
			sceneLighting.sceneLights[sceneLighting.numLights] = *(mSceneLights[i]);
			sceneLighting.numLights++;
		}

		// Set the lighting constant buffer
		mLightingBuffer->updateData(&sceneLighting);
		mLightingBuffer->bind(1, ShaderType::PixelShader);
	}
}