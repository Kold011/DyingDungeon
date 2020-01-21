#pragma once
#include "EngineIncludes.h"
#include "Component.h"
#include "Buffer.h"
#include "RenderState.h"
#include <random>
#include "ReadWriteLock.h"

namespace Odyssey
{
	// Forward declaration
	class Material;
	class Shader;
	class Texture;
	class RenderDevice;

	enum class EmissionType
	{
		None,
		Looping,
		Burst
	};

	struct ParticleSystemShape
	{
		DirectX::XMFLOAT3 center;
		float radius;
		float minXDirection, maxXDirection;
		float minYDirection, maxYDirection;
		float minZDirection, maxZDirection;
	};

	struct ConePS : public ParticleSystemShape
	{
		ConePS()
		{
			center = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			radius = 1.0f;

			minXDirection = -0.25f;
			maxXDirection = 0.25f;
			minYDirection = 1.0f;
			maxYDirection = 1.0f;
			minZDirection = -0.25f;
			maxZDirection = 0.25f;
		}

		ConePS(float posX, float posY, float posZ, float radius, float xAxisAngle, float zAxisAngle)
		{
			center = DirectX::XMFLOAT3(posX, posY, posZ);
			radius = radius;

			minXDirection = -xAxisAngle / 180.0f;
			maxXDirection = xAxisAngle / 180.0f;
			minYDirection = 1.0f;
			maxYDirection = 1.0f;
			minZDirection = -zAxisAngle / 180.0f;
			maxZDirection = zAxisAngle / 180.0f;
		}
	};

	struct SpherePS : public ParticleSystemShape
	{
		SpherePS()
		{
			center = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			radius = 1.0f;

			minXDirection = -1.0f;
			maxXDirection = 1.0f;
			minYDirection = -1.0f;
			maxYDirection = 1.0f;
			minZDirection = -1.0f;
			maxZDirection = 1.0f;
		}

		SpherePS(float x, float y, float z, float radius)
		{
			center = DirectX::XMFLOAT3(x, y, z);
			radius = radius;

			minXDirection = -1.0f;
			maxXDirection = 1.0f;
			minYDirection = -1.0f;
			maxYDirection = 1.0f;
			minZDirection = -1.0f;
			maxZDirection = 1.0f;
		}
	};
	class ParticleSystem : public Component
	{
		CLASS_DECLARATION(ParticleSystem)

	public: // Rule of 3
		ParticleSystem(RenderDevice& renderDevice);

	public: // Interface
		virtual void update(double deltaTime);
	public: // Helpers
		void run(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
		void setShape(ParticleSystemShape shape);
		void setTexture(TextureType textureType, const char* filename);
		void setColor(DirectX::XMFLOAT3 startColor, DirectX::XMFLOAT3 endColor);
		void setLifetime(float min, float max);
		void setParticleCount(int particleCount);
		void setRateOverTime(int emissionRate);
		void setSpeed(float min, float max);
		void setSize(float min, float max);
		DirectX::XMFLOAT3 getStartColor();
		DirectX::XMFLOAT3 getEndColor();
		void setLooping(bool looping);
		void setDuration(double duration);
		void play();
		void pause();
		void stop();
	private:
		void createDepthState();
		void createBlendState();
		void createParticleBuffer();
		void createParticle(Particle& particle);
		void generatePosition(Particle& particle);
		int runEmission(double deltaTime);
		DirectX::XMFLOAT4 colorLerp(DirectX::XMFLOAT3 startColor, DirectX::XMFLOAT3 endColor, float ratio);
	private: // Members
		ParticleSystemShape mShape;
		DirectX::XMFLOAT3 mStartColor;
		DirectX::XMFLOAT3 mEndColor;
		float mMinLife, mMaxLife;
		float mMinSpeed, mMaxSpeed;
		float mMinSize, mMaxSize;
		int mParticleCount, mMaxCount;
		int mEmissionRate;
		bool mIsLooping;
		bool mIsPlaying;
		double mDuration;
	private: // Rendering
		std::shared_ptr<Shader> mVertexShader;
		std::shared_ptr<Shader> mGeometryShader;
		std::shared_ptr<Shader> mPixelShader;
		std::shared_ptr<RenderState> mRenderState;
		std::vector<Particle> mParticleData;
		std::shared_ptr<Buffer> mParticleBuffer;
		std::shared_ptr<Texture> mTexture;
	private:
		RenderDevice& mRenderDevice;
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11BlendState> mBlendState;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mDepthDisabled;
		std::random_device rnd;
		ReadWriteLock mLock;
	};
}