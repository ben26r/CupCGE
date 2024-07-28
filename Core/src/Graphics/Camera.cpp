#include "Camera.h"

namespace Cup {

	void Camera::Resize(float width, float height)
	{
		aspectRatio = height / width;
	}

	const Matrix4x4f& Camera::GetView(const Vector3f position)
	{
		m_lookDir = Matrix4x4f::Rotation(Vector3f::Up(), m_yaw) * Vector3f::Far();
		m_right = (m_lookDir.cross(Vector3f::Up())).normalize();
		return Matrix4x4f::PointAt(position, position + m_lookDir, Vector3f::Down()).QuickInverse();
	}

}