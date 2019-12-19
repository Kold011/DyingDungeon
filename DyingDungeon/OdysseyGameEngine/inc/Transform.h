#pragma once
#include "EngineIncludes.h"
#include "Component.h"

namespace Odyssey
{
	class Transform : public Component
	{
		CLASS_DECLARATION(Transform)
	public:
		Transform();
		Transform(DirectX::XMFLOAT4X4 worldMatrix);
	public:
		/**
		 *	Initialize the Transform with default values. Called once when the scene is loaded.
		 *	@param[in] void
		 *	@return void
		 */
		virtual void initialize();

		/**
		 *	Add to the position of the Transform.
		 *	@param[in] x The x value to add to the position.
		 *	@param[in] y The y value to add to the position.
		 *	@param[in] z The z value to add to the position.
		 *	@return void
		 */
		void addPosition(float x, float y, float z);

		/**
		 *	Set the position of the Transform.
		 *	@param[in] x The x value of the position.
		 *	@param[in] y The y value of the position.
		 *	@param[in] z The z value of the position.
		 *	@return void
		 */
		void setPosition(float x, float y, float z);

		/**
		 *	Get the position of the Transform.
		 *	@param[out] position The position of the Transform.
		 *	@return void
		 */
		DirectX::XMFLOAT3 getPosition();

		/**
		 *	Add to the rotation of the Transform.
		 *	@param[in] x The x value to add to the rotation.
		 *	@param[in] y The y value to add to the rotation.
		 *	@param[in] z The z value to add to the rotation.
		 *	@return void
		 */
		void addRotation(float x, float y, float z);

		/**
		 *	Set the rotation of the Transform.
		 *	@param[in] x The x value of the rotation.
		 *	@param[in] y The y value of the rotation.
		 *	@param[in] z The z value of the rotation.
		 *	@return void
		 */
		void setRotation(float x, float y, float z);

		/**
		 *	Get the euler rotation of the Transform.
		 *	@param[out] rotation The euler rotation of the Transform.
		 *	@return void
		 */
		DirectX::XMFLOAT3 getEulerRotation();

		/**
		 *	Add to the scale of the Transform.
		 *	@param[in] x The x value to add to the scale.
		 *	@param[in] y The y value to add to the scale.
		 *	@param[in] z The z value to add to the scale.
		 *	@return void
		 */
		void addScale(float x, float y, float z);

		/**
		 *	Set the scale of the Transform.
		 *	@param[in] x The x value of the scale.
		 *	@param[in] y The y value of the scale.
		 *	@param[in] z The z value of the scale.
		 *	@return void
		 */
		void setScale(float x, float y, float z);

		/**
		 *	Get the scale of the Transform.
		 *	@param[out] rotation The scale of the Transform.
		 *	@return void
		 */
		DirectX::XMFLOAT3 getScale();

		/**
		 *	Get the forward direction of the Transform.
		 *	@param[out] forward the forward direction of the Transform.
		 *	@return void
		 */
		DirectX::XMFLOAT3 getForward();

		/**
		 *	Get the right direction of the Transform.
		 *	@param[out] forward the forward direction of the Transform.
		 *	@return void
		 */
		DirectX::XMFLOAT3 getRight();

		/**
		 *	Get the up direction of the Transform.
		 *	@param[out] forward the forward direction of the Transform.
		 *	@return void
		 */
		DirectX::XMFLOAT3 getUp();

		/**
		 *	Get the local-space matrix of the Transform
		 *	@param[out] localTransform The local-space matrix of the Transform
		 *	@return void
		 */
		DirectX::XMFLOAT4X4 getLocalTransform();

		/**
		 *	Get the global-space matrix of the Transform
		 *	@param[out] globalTransform The global-space matrix of the Transform
		 *	@return void
		 */
		DirectX::XMFLOAT4X4 getGlobalTransform();
	private:
		/**
		 *	Decompose the world matrix to determine the euler angle rotation.
		 *	@param[in] void
		 *	@return void
		 */
		void calculateEulerRotations();

		/**
		 *	Composes the position, rotation and scale into a world matrix.
		 *	@param[in] void
		 *	@return void
		 */
		void recalculateWorldMatrix();
	private:
		DirectX::XMFLOAT3 mPosition;
		DirectX::XMFLOAT3 mRotation;
		DirectX::XMFLOAT3 mScale;
		DirectX::XMFLOAT4X4 mWorldMatrix;
	};
}