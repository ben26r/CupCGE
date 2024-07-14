#include "Camera.h"

namespace Cup {

	void Camera::RecalulateView()
	{
		m_lookDir = Matrix4x4f::Rotation(Vector3f::Up(), m_yaw) * Vector3f::Far();
		m_viewMatrix = Matrix4x4f::PointAt(m_position, m_position + m_lookDir, Vector3f::Down()).QuickInverse();
	}

	void Camera::RecalulateProjection()
	{
		m_projectionMatrix = Matrix4x4f::Projection(m_fov, m_aspectRatio, m_near, m_far);
	}

}