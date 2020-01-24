#include "ParticleSystem.h"
#include "Buffer.h"
#include "Material.h"
#include "RenderState.h"
#include "RenderDevice.h"
#include "Shader.h"
#include "Texture.h"
#include "Entity.h"

namespace Odyssey
{
	CLASS_DEFINITION(Component, ParticleSystem)

		ParticleSystem::ParticleSystem(RenderDevice& renderDevice) : mRenderDevice(renderDevice)
	{
		// Transfer the direct3d device
		// TODO: REFACTOR THIS LATER BY MAKING A CUSTOM DEPTH AND BLEND STATE CLASS
		mDevice = renderDevice.getDevice();

		// Create the shaders
		mVertexShader = renderDevice.createShader(ShaderType::VertexShader, "../OdysseyEngine/shaders/ParticleVertexShader.cso", nullptr, 0);
		mGeometryShader = renderDevice.createShader(ShaderType::GeometryShader, "../OdysseyEngine/shaders/ParticleGeometryShader.cso", nullptr);
		mPixelShader = renderDevice.createShader(ShaderType::PixelShader, "../OdysseyEngine/shaders/ParticlePixelShader.cso", nullptr);

		// Create the render state
		mRenderState = renderDevice.createRenderState(Topology::PointList, CullMode::CULL_NONE, FillMode::FILL_SOLID, false, false, false);

		// Create the depth and blend states
		createDepthState();
		createBlendState();

		// Default to the fire texture
		mTexture = renderDevice.createTexture(TextureType::Diffuse, "Particle.png");

		// Create the particle data buffer
		mStartCount = 10;
		mMaxCount = 100;
		mEmissionRate = 10;
		mShape = SpherePS();
		mStartColor = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		mEndColor = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		mMinSpeed = 0.0f;
		mMaxSpeed = 1.0f;
		mMinSize = 0.25f;
		mMaxSize = 1.0f;
		mMinLife = 0.0f;
		mMaxLife = 1.0f;
		mGravity = 0.0f;
		mGravityEnabled = false;
		mIsPlaying = true;
		time = 0.0;
		totalTime = 1.0;
		numSpawn = mEmissionRate;
		mCurrentEmission = 0;
	}

	void ParticleSystem::run(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		mLock.lock(LockState::Read);

		// Unbind any vertex buffers from the pipeline
		ID3D11Buffer* nBuffs[] = { nullptr };
		UINT stride = 0;
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, nBuffs, &stride, &offset);

		// Bind the depth state
		context->OMSetDepthStencilState(mDepthDisabled.Get(), 0);

		// Update the particle buffer
		mParticleBuffer->updateData(context, mParticleData.data());

		// Bind the geometry shader
		mGeometryShader->bind(context);

		// Bind the particle buffer
		mParticleBuffer->bind(context, 0, ShaderType::GeometryShader);

		// Bind the render state
		mRenderState->bind(context);

		// Bind the vertex shader and material properties
		mVertexShader->bind(context);
		mPixelShader->bind(context);

		// Bind the particle texture
		mTexture->bind(context);

		// Bind the blend state
		float blendFactor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		context->OMSetBlendState(mBlendState.Get(), blendFactor, 0xffffffff);

		// Draw the particles
		context->Draw(mMaxCount, 0);

		// Unbind the depth and blend state
		context->OMSetDepthStencilState(nullptr, 0);
		context->OMSetBlendState(0, 0, 0xffffffff);

		// Unbind the shaders and buffers
		mParticleBuffer->unbind(context, 0, ShaderType::GeometryShader);
		mGeometryShader->unbind(context);
		mPixelShader->unbind(context);
		mVertexShader->unbind(context);

		mLock.unlock(LockState::Read);
	}

	void ParticleSystem::setShape(ParticleSystemShape shape)
	{
		mShape = shape;
	}

	void ParticleSystem::setTexture(TextureType textureType, const char* filename)
	{
		mTexture = mRenderDevice.createTexture(textureType, filename);
	}

	void ParticleSystem::setColor(DirectX::XMFLOAT3 startColor, DirectX::XMFLOAT3 endColor)
	{
		mStartColor = startColor;
		mEndColor = endColor;
	}

	void ParticleSystem::setLifetime(float min, float max)
	{
		mMinLife = min;
		mMaxLife = max;
	}

	void ParticleSystem::setParticleCount(int startingCount, int maxCount)
	{
		mStartCount = startingCount;
		mMaxCount = maxCount;
		mParticleData.resize(mMaxCount);
		createParticleBuffer();
	}

	void ParticleSystem::setRateOverTime(int emissionRate)
	{
		mEmissionRate = emissionRate;
		numSpawn = mEmissionRate;
	}

	void ParticleSystem::setSpeed(float min, float max)
	{
		mMinSpeed = min;
		mMaxSpeed = max;
	}

	void ParticleSystem::setSize(float min, float max)
	{
		mMinSize = min;
		mMaxSize = max;
	}

	DirectX::XMFLOAT3 ParticleSystem::getStartColor()
	{
		return mStartColor;
	}

	DirectX::XMFLOAT3 ParticleSystem::getEndColor()
	{
		return mEndColor;
	}

	void ParticleSystem::setLooping(bool looping)
	{
		mIsLooping = looping;
	}

	void ParticleSystem::setDuration(double duration)
	{
		mDuration = duration;
	}

	void ParticleSystem::setGravity(float gravity)
	{
		mGravity = gravity;
		mGravityEnabled = true;
	}

	void ParticleSystem::play()
	{
		mIsActive = true;
		mIsPlaying = true;
		mCurrentTime = 0.0f;

		setInitialData();
	}

	void ParticleSystem::pause()
	{
		mIsPlaying = false;
	}

	void ParticleSystem::stop()
	{
		mIsActive = false;
		mIsPlaying = false;
		mParticleData.clear();
		mParticleData.resize(mMaxCount);
		mCurrentTime = 0.0f;
	}

	void ParticleSystem::createDepthState()
	{
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilDesc.StencilEnable = false;
		depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		// Stencil operations if pixel is front-facing
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		HRESULT hr = mDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthDisabled);
	}

	void ParticleSystem::createBlendState()
	{
		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));

		D3D11_RENDER_TARGET_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.BlendEnable = true;
		desc.BlendEnable = true;
		desc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.DestBlend = D3D11_BLEND_ONE;
		desc.BlendOp = D3D11_BLEND_OP_ADD;
		desc.SrcBlendAlpha = D3D11_BLEND_ZERO;
		desc.DestBlendAlpha = D3D11_BLEND_ONE;
		desc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		blendDesc.AlphaToCoverageEnable = true;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0] = desc;

		mDevice->CreateBlendState(&blendDesc, mBlendState.GetAddressOf());
	}

	void ParticleSystem::setInitialData()
	{
		for (int i = 0; i < mStartCount; i++)
		{
			Particle p;
			createParticle(p);
			mParticleData[i] = p;
		}
	}

	void ParticleSystem::createParticleBuffer()
	{
		if (mParticleData.size() != mMaxCount)
			mParticleData.resize(mMaxCount);

		if (mParticleBuffer)
			mParticleBuffer = nullptr;

		mParticleBuffer = mRenderDevice.createBuffer(BufferBindFlag::StructuredBuffer, size_t(mMaxCount),
			static_cast<UINT>(sizeof(Particle)), mParticleData.data());
	}

	void ParticleSystem::createParticle(Particle& particle)
	{
		std::mt19937 generator(rnd());
		std::uniform_real_distribution<float> xPosition(-1.0f * mShape.radius.x + mShape.center.x, mShape.center.x + mShape.radius.x);
		std::uniform_real_distribution<float> yPosition(-1.0f * mShape.radius.y + mShape.center.y, mShape.center.y + mShape.radius.y);
		std::uniform_real_distribution<float> zPosition(-1.0f * mShape.radius.z + mShape.center.z, mShape.center.z + mShape.radius.z);
		std::uniform_real_distribution<float> xDirection(mShape.minXDirection, mShape.maxXDirection);
		std::uniform_real_distribution<float> yDirection(mShape.minYDirection, mShape.maxYDirection);
		std::uniform_real_distribution<float> zDirection(mShape.minZDirection, mShape.maxZDirection);
		std::uniform_real_distribution<float> lf(mMinLife, mMaxLife);
		std::uniform_real_distribution<float> spd(mMinSpeed, mMaxSpeed);
		std::uniform_real_distribution<float> size(mMinSize, mMaxSize);
		float speed = spd(generator);

		particle.color = colorLerp(mStartColor, mEndColor, 0.0f);
		particle.position = DirectX::XMFLOAT3(xPosition(generator), yPosition(generator), zPosition(generator));
		particle.velocity = DirectX::XMFLOAT3(xDirection(generator), yDirection(generator), zDirection(generator));
		DirectX::XMStoreFloat3(&particle.velocity, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&particle.velocity)));
		particle.velocity = DirectX::XMFLOAT3(particle.velocity.x * speed, particle.velocity.y * speed, particle.velocity.z * speed);
		particle.size = size(generator);
		particle.active = true;
		particle.lifeTime = lf(generator);
		particle.startLifetime = particle.lifeTime;
	}

	int ParticleSystem::runEmission(double deltaTime)
	{
		int spawn = 0;

		double emissionInterval = totalTime / (double)numSpawn;

		time += deltaTime;

		if (time > emissionInterval)
		{
			spawn = floor(time / emissionInterval);
			spawn = max(0, min(spawn, mEmissionRate));
			totalTime -= time;
			numSpawn -= spawn;
			time = 0.0;
		}

		if (totalTime <= 0.0)
		{
			totalTime = 1.0;
			numSpawn = mEmissionRate;
		}
		return spawn;
	}

	DirectX::XMFLOAT4 ParticleSystem::colorLerp(DirectX::XMFLOAT3 startColor, DirectX::XMFLOAT3 endColor, float ratio)
	{
		DirectX::XMFLOAT4 finalColor;
		finalColor.x = (1.0f - ratio) * startColor.x + (ratio * endColor.x);
		finalColor.y = (1.0f - ratio) * startColor.y + (ratio * endColor.y);
		finalColor.z = (1.0f - ratio) * startColor.z + (ratio * endColor.z);
		finalColor.w = 1.0f - ratio;
		// Return the resulting lerped color
		return finalColor;
	}

	float ParticleSystem::lerp(float start, float end, float ratio)
	{
		return (1.0f - ratio) * start + (ratio * end);
	}

	void ParticleSystem::initialize()
	{
		if (mIsPlaying)
			setInitialData();
	}

	void ParticleSystem::update(double deltaTime)
	{
		mLock.lock(LockState::Write);

		if (mCurrentTime >= mDuration && mIsPlaying && !mIsLooping)
		{
			mCurrentGravity = 0.0f;
			stop();
			mLock.unlock(LockState::Write);
			return;
		}

		if (mIsPlaying == false)
		{
			mLock.unlock(LockState::Write);
			return;
		}

		mCurrentTime += deltaTime;

		bool allowEmit = (mIsLooping || mCurrentTime < mDuration);

		mCurrentEmission = runEmission(deltaTime);

		for (int i = 0; i < mParticleData.size(); i++)
		{
			mParticleData[i].lifeTime -= deltaTime;

			mCurrentGravity = lerp(0.0f, mGravity, (1.0f - mParticleData[i].lifeTime / mParticleData[i].startLifetime));

			if (mParticleData[i].lifeTime <= 0.0f && mParticleData[i].active)
			{
				mParticleData[i].active = false;
				mParticleData[i].color = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
			}
			else if (mParticleData[i].active)
			{
				mParticleData[i].position.x += mParticleData[i].velocity.x * deltaTime;
				mParticleData[i].position.y += mParticleData[i].velocity.y * deltaTime;
				mParticleData[i].position.z += mParticleData[i].velocity.z * deltaTime;

				if (mGravityEnabled)
					mParticleData[i].position.y -= mCurrentGravity * deltaTime;

				float ratio = 1.0f - mParticleData[i].lifeTime / mParticleData[i].startLifetime;

				mParticleData[i].color = colorLerp(mStartColor, mEndColor, ratio);
			}

			if (mParticleData[i].active == false && mCurrentEmission != 0 && allowEmit)
			{
				createParticle(mParticleData[i]);
				mCurrentEmission--;
			}
		}

		mLock.unlock(LockState::Write);
	}
}