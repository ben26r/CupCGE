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
		Camera(float aspectRatio, const CameraProps& props = CameraProps()) :
			m_near(props.cnear), m_far(props.cfar), m_fov(props.fov), m_aspectRatio(aspectRatio),
			m_projectionMatrix(Matrix4x4::Projection(props.fov, aspectRatio, props.cnear, props.cfar)) {}
		~Camera() = default;

		inline const Matrix4x4& GetProjection() const { return m_projectionMatrix; }
		inline const Matrix4x4& GetView() const { return m_viewMatrix; }

		inline const Vector3& GetPosition() const { return m_position; }
		inline void SetPosition(const Vector3& position) { m_position = position; RecalulateView(); }

		inline void SetFOV(float fov) { m_fov = fov; }

		void RecalulateView();
		void RecalulateProjection();

		float m_near;
		float m_far;
		float m_fov;
		float m_aspectRatio;

		float m_yaw = 0.0f;
		Vector3 m_lookDir;
		Vector3 m_position;

		Matrix4x4 m_projectionMatrix;
		Matrix4x4 m_viewMatrix;
	};

}