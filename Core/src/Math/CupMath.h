#pragma once

#include <functional>
#include <iostream>
#include <cmath>

namespace Cup
{
    struct Vector3 {
        float x = 0;
        float y = 0;
        float z = 0;
        float w = 1;

        Vector3() = default;
        Vector3(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
        
        inline static Vector3 Up()    { return Vector3( 0,  1,  0); }
        inline static Vector3 Down()  { return Vector3( 0, -1,  0); }
        inline static Vector3 Left()  { return Vector3(-1,  0,  0); }
        inline static Vector3 Right() { return Vector3( 1,  0,  0); }
        inline static Vector3 Far()   { return Vector3( 0,  0,  1); }
        inline static Vector3 Near()  { return Vector3( 0,  0, -1); }

        Vector3 Rotate(const Vector3& axis, const float theta);

        Vector3 PreRotate(const Vector3& axis, const float theta, const float rotx, const float roty);

        // Addition
        Vector3 operator+(const Vector3& other) const {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }

        void operator+=(const Vector3& other) {
            x += other.x; y += other.y; z += other.z;
        }

        // Subtraction
        Vector3 operator-(const Vector3& other) const {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }

        void operator-=(const Vector3& other) {
            x -= other.x; y -= other.y; z -= other.z;
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
        float matrix[4][4] = { 0 };

        float* operator[](int index) { return matrix[index]; }
        const float* operator[](int index) const { return matrix[index]; }

        static Matrix4x4 Identity();

        static Matrix4x4 Scale(float x, float y, float z);

        static Matrix4x4 Translation(float x, float y, float z);

        static Matrix4x4 Zero();

        static Matrix4x4 Projection(float fovDegrees, float aspectRatio, float cnear, float cfar);

        static Matrix4x4 Rotation(const Vector3& axis, float angle);

        static Matrix4x4 PointAt(const Vector3& position, const Vector3& target, const Vector3& up);

        Matrix4x4 QuickInverse(); // Only for Rotation/Translation Matrices

        // Addition operator
        Matrix4x4 operator+(const Matrix4x4& other) const {
            Matrix4x4 result;
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    result.matrix[i][j] = matrix[i][j] + other.matrix[i][j];
                }
            }
            return result;
        }

        // Multiplication by scalar
        Matrix4x4 operator*(float scalar) const {
            Matrix4x4 result;
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    result.matrix[i][j] = matrix[i][j] * scalar;
                }
            }
            return result;
        }

        Vector3 operator*(const Vector3& other) const {
            Vector3 result;
            result.x = other.x * matrix[0][0] + other.y * matrix[1][0] + other.z * matrix[2][0] + other.w * matrix[3][0];
            result.y = other.x * matrix[0][1] + other.y * matrix[1][1] + other.z * matrix[2][1] + other.w * matrix[3][1];
            result.z = other.x * matrix[0][2] + other.y * matrix[1][2] + other.z * matrix[2][2] + other.w * matrix[3][2];
            result.w = other.x * matrix[0][3] + other.y * matrix[1][3] + other.z * matrix[2][3] + other.w * matrix[3][3];

            return result;
        }

        // Matrix multiplication
        Matrix4x4 operator*(const Matrix4x4& other) const {
            Matrix4x4 result;
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    result.matrix[i][j] = 0.0f;
                    for (int k = 0; k < 4; ++k) {
                        result.matrix[i][j] += matrix[i][k] * other.matrix[k][j];
                    }
                }
            }
            return result;
        }

    };

    struct Triangle
    {
        Vector3 vertices[3];
        Vector4 color;

        void Do(const std::function<void(Vector3&)>& func)
        {
            for (Vector3& vertex : vertices)
                func(vertex);
        }

        void operator*=(const Matrix4x4& other)
        {
            vertices[0] = other * vertices[0];
            vertices[1] = other * vertices[1];
            vertices[2] = other * vertices[2];
        }

        Triangle operator*(const Matrix4x4& other) const
        {
            Triangle result;
            result[0] = other * vertices[0];
            result[1] = other * vertices[1];
            result[2] = other * vertices[2];
            return result;
        }

        Vector3& operator[](int index) { return vertices[index]; }
        const Vector3& operator[](int index) const { return vertices[index]; }
    };

    //how to lines intersect with planes
    Vector3 IntersectPlane(Vector3& plane_p, Vector3& plane_n, Vector3& lineStart, Vector3& lineEnd);

    int ClipAgainstPlane(Vector3 plane_p, Vector3 plane_n, Triangle& in_tri, Triangle& out_tri1, Triangle& out_tri2);

    void MultiplyVectorMatrix(Vector3& vector, const Matrix4x4& matrix);

    Vector3 MultiplyVectorMatrix(const Vector3& vector, const Matrix4x4& matrix);
}

