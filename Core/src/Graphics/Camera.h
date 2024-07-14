#pragma once

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
		Camera() = default;
		Camera(float aspectRatio, const CameraProps& props = CameraProps()) :
			m_near(props.cnear), m_far(props.cfar), m_fov(props.fov), m_aspectRatio(aspectRatio),
			m_projectionMatrix(Matrix4x4f::Projection(props.fov, aspectRatio, props.cnear, props.cfar)) {}
		~Camera() = default;

		inline const Matrix4x4f& GetProjection() const { return m_projectionMatrix; }
		inline const Matrix4x4f& GetView() const { return m_viewMatrix; }

		inline const Vector3f& GetPosition() const { return m_position; }
		inline void SetPosition(const Vector3f& position) { m_position = position; RecalulateView(); }

		inline void SetFOV(float fov) { m_fov = fov; }

		void RecalulateView();
		void RecalulateProjection();

		float m_near;
		float m_far;
		float m_fov;
		float m_aspectRatio;

		float m_yaw = 0.0f;
		Vector3f  m_lookDir;
		Vector3f m_position;

		Matrix4x4f m_projectionMatrix;
		Matrix4x4f m_viewMatrix;
	};

}