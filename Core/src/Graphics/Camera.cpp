#include "Camera.h"

namespace Cup {

	void Camera::RecalulateView()
	{
		m_lookDir = Matrix4x4::Rotation(Vector3::Up(), m_yaw) * Vector3::Far();
		m_viewMatrix = Matrix4x4::PointAt(m_position, m_position + m_lookDir, Vector3::Down()).QuickInverse();
	}

	void Camera::RecalulateProjection()
	{
		m_projectionMatrix = Matrix4x4::Projection(m_fov, m_aspectRatio, m_near, m_far);
	}

}