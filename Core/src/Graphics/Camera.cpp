#include "Camera.h"

namespace Cup {

	void Camera::Resize(float width, float height)
	{
		aspectRatio = height / width;
	}

	void Camera::RecalulateView()
	{
		m_lookDir = Matrix4x4f::Rotation(Vector3f::Up(), m_yaw) * Vector3f::Far();
		m_viewMatrix = Matrix4x4f::PointAt(m_position, m_position + m_lookDir, Vector3f::Down()).QuickInverse();
	}

}