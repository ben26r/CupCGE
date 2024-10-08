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
		Camera(float _aspectRatio = 0.8f, const CameraProps& props = CameraProps()) :
			cnear(props.cnear), cfar(props.cfar), fov(props.fov), aspectRatio(_aspectRatio) {}
		~Camera() = default;

		inline const Matrix4x4f& GetProjection() const { return Matrix4x4f::Projection(fov, aspectRatio, cnear, cfar); }
		const Matrix4x4f& GetView(const Vector3f position);

		inline void SetFOV(float fov) { fov = fov; }

		void Resize(float width, float height);

		float cnear;
		float cfar;
		float fov;
		float aspectRatio;

		float m_yaw = 0.0f;
		Vector3f  m_lookDir;
		Vector3f m_right;
	};

}