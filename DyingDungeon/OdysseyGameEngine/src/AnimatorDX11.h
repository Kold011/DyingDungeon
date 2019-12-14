#pragma once
#include "EngineIncludes.h"
#include "Animator.h"

namespace Odyssey
{
	class Buffer;
	class RenderDevice;

	class AnimatorDX11 : public Animator
	{
		CLASS_DECLARATION(AnimatorDX11)
	public:
		/**
		 *	Construct an AnimatorDX11 component.
		 *	@param[in] renderDevice: A reference to the render device used to create and bind rendering resources.
		 */
		AnimatorDX11(RenderDevice& renderDevice);

		/**
		 *	Initialize the AnimatorDX11 component with default values. Called once when the scene is loaded.
		 *	@param[in] void
		 *	@return void
		 */
		virtual void initialize();

		/**
		 *	Update the Animator component. Called once per frame when the scene is updated.
		 *	@param[in] deltaTime The time between frames.
		 *	@return void
		 */
		virtual void update(double deltaTime);

		/**
		 *	Bind the animated joint matrices to the vertex shader.
		 *	@param[in] void
		 *	@return void
		 */
		void bind();

		/**
		 *	Unbind the animated joint matrices from the vertex shader.
		 *	@param[in] void
		 *	@return void
		 */
		void unbind();

		/**
		 *	Draw the skeleton and joints in wireframe mode.
		 *	@param[in] globalTransform The object's transform in global space
		 *	@param[in] color The color used to render the skeleton and joints
		 *	@return void
		 */
		void debugDraw(DirectX::XMFLOAT4X4& globalTransform, DirectX::XMFLOAT3 color);

		/**
		 *	Set the skeleton attached to the Animator component
		 *	@param[in] skeleton The container of joints associated with the skeelton.
		 *	@return void
		 */
		void setSkeleton(std::vector<Joint> skeleton);

	private:
		/**
		 *	Update the current animation sequence, if one is set. Called once per frame when the scene is updated.
		 *	@param[in] deltaTime The time between frames.
		 *	@return void
		 */
		void updateSequence(double deltaTime);

		/**
		 *	Update an animation clip to reflect the change in time and frame.
		 *	@param[in] clip The animation clip to update.
		 *	@param[in] deltaTime The time between frames.
		 *	@param[out] outPrevKeyframe The calculated previous keyframe for blending.
		 *	@param[out] outNextKeyframe The calculated next keyframe for blending.
		 *	@return void
		 */
		void updateAnimationTime(AnimationClip& clip, double deltaTime, Keyframe& outPrevKeyframe, Keyframe& outNextKeyframe);

		/**
		 *	Blend two keyframes of an animation clip based on a blend factor.
		 *	@param[in] keyframeA The first keyframe for blending.
		 *	@param[in] keyframeB The second keyframe for blending.
		 *	@param[in] blendFactor The amount of blending to apply.
		 *	@param[out] outKeyframe The keyframe computed from the blending process.
		 *	@return void
		 */
		void blendKeyframes(Keyframe& keyframeA, Keyframe& keyframeB, float blendFactor, Keyframe& outKeyframe);

		/**
		 *	Update the shader buffer storing the animated keyframe matrices.
		 *	@param[in] animationKeyframe The current animation keyframe to send to the shader.
		 *	@return void
		 */
		void updateAnimationBuffer(Keyframe& animationKeyframe);

		/**
		 *	Perform Spherical linear interpolation between two joint matrices of a keyframe.
		 *	@param[in] prevTransform The matrix of the joint in the previous frame.
		 *	@param[in] nextTransform The matrix of the joint in the next frame.
		 *	@param[in] ratio The amount of slerp to apply.
		 *	@param[out] outTransform The matrix of the joint after slerp.
		 *	@return void
		 */
		void interpolateJointMatrix(DirectX::XMFLOAT4X4& prevTransform, DirectX::XMFLOAT4X4& nextTransform, float ratio, DirectX::XMFLOAT4X4& outTransform);

	private:
		std::shared_ptr<Buffer> mAnimationBuffer;
		AnimationBuffer mAnimationData;
		std::vector<Joint> mSkeleton;
	};
}