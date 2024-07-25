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
			cnear(props.cnear), cfar(props.cfar), fov(props.fov), aspectRatio(aspectRatio) {}
		~Camera() = default;

		inline const Matrix4x4f& GetProjection() const { return Matrix4x4f::Projection(fov, aspectRatio, cnear, cfar); }
		inline const Matrix4x4f& GetView() const { return m_viewMatrix; }

		inline const Vector3f& GetPosition() const { return m_position; }
		inline void SetPosition(const Vector3f& position) { m_position = position; RecalulateView(); }

		inline void SetFOV(float fov) { fov = fov; }

		void Resize(float width, float height);
		void RecalulateView();

		float cnear;
		float cfar;
		float fov;
		float aspectRatio;

		float m_yaw = 0.0f;
		Vector3f  m_lookDir;
		Vector3f m_right;
		Vector3f m_position;

		Matrix4x4f m_viewMatrix;
	};

}