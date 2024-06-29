#pragma once

#include <iostream>
#include "cmath"

namespace Cup
{
    struct Vector3 {
        float x = 0;
        float y = 0;
        float z = 0;

        Vector3() = default;
        Vector3(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
        
        void operator=(const Vector3& other) {
            x = other.x, y = other.y, z = other.z;
        }

        // Addition
        Vector3 operator+(const Vector3& other) const {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }

        // Subtraction
        Vector3 operator-(const Vector3& other) const {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }

        // Scalar multiplication
        Vector3 operator*(float scalar) const {
            return Vector3(x * scalar, y * scalar, z * scalar);
        }

        // Scalar division
        Vector3 operator/(float scalar) const {
            return Vector3(x / scalar, y / scalar, z / scalar);
        }

        // Dot product
        float dot(const Vector3& other) const {
            return x * other.x + y * other.y + z * other.z;
        }

        // Cross product
        Vector3 cross(const Vector3& other) const {
            return Vector3(
                y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x
            );
        }

        // Magnitude
        float magnitude() const {
            return std::sqrt(x * x + y * y + z * z);
        }

        // Normalize
        Vector3 normalize() const {
            float mag = magnitude();
            return Vector3(x / mag, y / mag, z / mag);
        }

        // Output stream overload
        friend std::ostream& operator<<(std::ostream& os, const Vector3& vec) {
            os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
            return os;
        }
    };

    struct Vector4 {
        float x = 0;
        float y = 0;
        float z = 0;
        float w = 0;

        Vector4() = default;
        Vector4(float X, float Y, float Z, float W) : x(X), y(Y), z(Z), w(W) {}

        // Addition
        Vector4 operator+(const Vector4& other) const {
            return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
        }

        // Subtraction
        Vector4 operator-(const Vector4& other) const {
            return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
        }

        // Scalar multiplication
        Vector4 operator*(float scalar) const {
            return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
        }

        // Scalar division
        Vector4 operator/(float scalar) const {
            return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
        }

        // Dot product
        float dot(const Vector4& other) const {
            return x * other.x + y * other.y + z * other.z + w * other.w;
        }

        // Magnitude
        float magnitude() const {
            return std::sqrt(x * x + y * y + z * z + w * w);
        }

        // Normalize
        Vector4 normalize() const {
            float mag = magnitude();
            return Vector4(x / mag, y / mag, z / mag, w / mag);
        }

        // Output stream overload
        friend std::ostream& operator<<(std::ostream& os, const Vector4& vec) {
            os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
            return os;
        }
    };

	struct Matrix4x4
	{
		float mat[4][4] = { 0 };

		float* operator[](int index) { return mat[index]; }
		const float* operator[](int index) const { return mat[index]; }
	};

	void MultiplyVectorMatrix(Vector3& vector, const Matrix4x4& matrix)
	{
		Vector3 result;
		result.x = vector.x * matrix[0][0] + vector.y * matrix[1][0] + vector.z * matrix[2][0] + matrix[3][0];
		result.y = vector.x * matrix[0][1] + vector.y * matrix[1][1] + vector.z * matrix[2][1] + matrix[3][1];
		result.z = vector.x * matrix[0][2] + vector.y * matrix[1][2] + vector.z * matrix[2][2] + matrix[3][2];
		float w  = vector.x * matrix[0][3] + vector.y * matrix[1][3] + vector.z * matrix[2][3] + matrix[3][3];

		if (w != 0.0f)
		{
			result = result / w;
		}

        vector = result;
	}

    Vector3 MultiplyVectorMatrix(const Vector3& vector, const Matrix4x4& matrix)
    {
        Vector3 result;
        result.x = vector.x * matrix[0][0] + vector.y * matrix[1][0] + vector.z * matrix[2][0] + matrix[3][0];
        result.y = vector.x * matrix[0][1] + vector.y * matrix[1][1] + vector.z * matrix[2][1] + matrix[3][1];
        result.z = vector.x * matrix[0][2] + vector.y * matrix[1][2] + vector.z * matrix[2][2] + matrix[3][2];
        float w = vector.x * matrix[0][3] + vector.y * matrix[1][3] + vector.z * matrix[2][3] + matrix[3][3];

        if (w != 0.0f)
        {
            result = result / w;
        }

        return result;
    }
}

