#pragma once

#include "Core/CupEngine.h"
#include "Math/CupMath.h"

namespace Cup {

	struct CameraProps
	{
		float cnear = 0.1f;
		float cfar = 1000.0f;
		float fov = 90.0f;
	};

	class Camera
	{
	public:
		Camera(const CameraProps& props, float aspectRatio) : 
			m_near(props.cnear), m_far(props.cfar), m_fov(props.fov), m_aspectRatio(aspectRatio),
			m_projectionMatrix(Matrix4x4::Projection(props.fov, aspectRatio, props.cnear, props.cfar)) {}
		~Camera() = default;



	private:
		float m_near;
		float m_far;
		float m_fov;
		float m_aspectRatio;

		float m_lookDir;

		Matrix4x4 m_projectionMatrix;
		Matrix4x4 m_viewMatrix;
	};

}