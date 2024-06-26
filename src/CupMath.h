#pragma once

namespace Cup
{
	struct Vector3
	{
		float x = 0;
		float y = 0;
		float z = 0;
	};

	struct Matrix4x4
	{
		float mat[4][4] = { 0 };

		float* operator[](int index) { return mat[index]; }
		const float* operator[](int index) const { return mat[index]; }
	};

	Vector3 MultiplyVectorMatrix(const Vector3& vector, const Matrix4x4& matrix)
	{
		Vector3 result;
		result.x = vector.x * matrix[0][0] + vector.y * matrix[1][0] + vector.z * matrix[2][0] + matrix[3][0];
		result.y = vector.x * matrix[0][1] + vector.y * matrix[1][1] + vector.z * matrix[2][1] + matrix[3][1];
		result.z = vector.x * matrix[0][2] + vector.y * matrix[1][2] + vector.z * matrix[2][2] + matrix[3][2];
		float w  = vector.x * matrix[0][3] + vector.y * matrix[1][3] + vector.z * matrix[2][3] + matrix[3][3];

		if (w != 0.0f)
		{
			result.x /= w;
			result.y /= w;
			result.z /= w;
		}

		return result;
	}
}

