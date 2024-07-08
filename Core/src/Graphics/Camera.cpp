#include "Camera.h"

namespace Cup {

	void Camera::RecalulateView()
	{
		m_lookDir = Matrix4x4<float>::Rotation(Vector3<float>::Up(), m_yaw) * Vector3<float>::Far();
		m_viewMatrix = Matrix4x4<float>::PointAt(m_position, m_position + m_lookDir, Vector3<float>::Down()).QuickInverse();
	}

	void Camera::RecalulateProjection()
	{
		m_projectionMatrix = Matrix4x4<float>::Projection(m_fov, m_aspectRatio, m_near, m_far);
	}

}