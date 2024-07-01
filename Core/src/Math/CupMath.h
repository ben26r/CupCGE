#pragma once

#include <iostream>
#include "cmath"

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
        float matrix[4][4] = { 0 };

        float* operator[](int index) { return matrix[index]; }
        const float* operator[](int index) const { return matrix[index]; }

        // Static function to create an identity matrix
        static Matrix4x4 Identity() {
            Matrix4x4 result;
            for (int i = 0; i < 4; ++i) {
                result.matrix[i][i] = 1.0f;
            }
            return result;
        }

        // Static function to create a zero matrix
        static Matrix4x4 Zero() {
            return Matrix4x4();
        }

        static Matrix4x4 Projection(float fovDegrees, float aspectRatio, float cnear, float cfar)
        {
            float fFovRad = 1.0f / tanf(fovDegrees * 0.5f / 180.0f * 3.14159f);
            Matrix4x4 result;
            result[0][0] = aspectRatio * fFovRad;
            result[1][1] = fFovRad;
            result[2][2] = cfar / (cfar - cnear);
            result[3][2] = (-cfar * cnear) / (cfar - cnear);
            result[2][3] = 1.0f;
            result[3][3] = 0.0f;
            return result;
        }

        static Matrix4x4 Rotation(const Vector3& axis, float angle)
        {
            Matrix4x4 matrix;

            if (axis.x == 1)
            {
                matrix[0][0] = 1.0f;
                matrix[1][1] = cosf(angle);
                matrix[1][2] = sinf(angle);
                matrix[2][1] = -sinf(angle);
                matrix[2][2] = cosf(angle);
                matrix[3][3] = 1.0f;
            }
            if (axis.y == 1)
            {
                matrix[0][0] = cosf(angle);
                matrix[0][2] = sinf(angle);
                matrix[2][0] = -sinf(angle);
                matrix[1][1] = 1.0f;
                matrix[2][2] = cosf(angle);
                matrix[3][3] = 1.0f;
            }
            if (axis.z == 1)
            {
                matrix[0][0] = cosf(angle);
                matrix[0][1] = sinf(angle);
                matrix[1][0] = -sinf(angle);
                matrix[1][1] = cosf(angle);
                matrix[2][2] = 1.0f;
                matrix[3][3] = 1.0f;
            }

            return matrix;
        }

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

        Vector3 operator*(const Vector3& vector) const {
            Vector3 result;
            result.x = vector.x * matrix[0][0] + vector.y * matrix[1][0] + vector.z * matrix[2][0] + vector.w * matrix[3][0];
            result.y = vector.x * matrix[0][1] + vector.y * matrix[1][1] + vector.z * matrix[2][1] + vector.w * matrix[3][1];
            result.z = vector.x * matrix[0][2] + vector.y * matrix[1][2] + vector.z * matrix[2][2] + vector.w * matrix[3][2];
            result.w = vector.x * matrix[0][3] + vector.y * matrix[1][3] + vector.z * matrix[2][3] + vector.w * matrix[3][3];

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

        static Matrix4x4 PointAt(const Vector3& position, const Vector3& target, const Vector3& up)
        {
            Vector3 newForward = target - position;
            newForward = newForward.normalize();

            Vector3 a = newForward * up.dot(newForward);
            Vector3 newUp = (up - a).normalize();

            Vector3 newRight = newUp.cross(newForward);

            Matrix4x4 result;
            result[0][0] = newRight.x;	    result[0][1] = newRight.y;	    result[0][2] = newRight.z;	    result[0][3] = 0.0f;
            result[1][0] = newUp.x;		    result[1][1] = newUp.y;		    result[1][2] = newUp.z;		    result[1][3] = 0.0f;
            result[2][0] = newForward.x;	result[2][1] = newForward.y;	result[2][2] = newForward.z;	result[2][3] = 0.0f;
            result[3][0] = position.x;		result[3][1] = position.y;		result[3][2] = position.z;		result[3][3] = 1.0f;
            return result;
        }

        Matrix4x4 QuickInverse() // Only for Rotation/Translation Matrices
        {
            Matrix4x4 result;
            result[0][0] =   matrix[0][0];  result[0][1] = matrix[1][0];  result[0][2] = matrix[2][0];  result[0][3] = 0.0f;
            result[1][0] =   matrix[0][1];  result[1][1] = matrix[1][1];  result[1][2] = matrix[2][1];  result[1][3] = 0.0f;
            result[2][0] =   matrix[0][2];  result[2][1] = matrix[1][2];  result[2][2] = matrix[2][2];  result[2][3] = 0.0f;
            result[3][0] = -(matrix[3][0] * result[0][0] + matrix[3][1] * result[1][0] + matrix[3][2] * result[2][0]);
            result[3][1] = -(matrix[3][0] * result[0][1] + matrix[3][1] * result[1][1] + matrix[3][2] * result[2][1]);
            result[3][2] = -(matrix[3][0] * result[0][2] + matrix[3][1] * result[1][2] + matrix[3][2] * result[2][2]);
            result[3][3] = 1.0f;
            return result;
        }

        // Print function for convenience
        void print() const {
            for (const auto& row : matrix) {
                for (const auto& val : row) {
                    std::cout << val << " ";
                }
                std::cout << std::endl;
            }
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

        Vector3& operator[](int index) { return vertices[index]; }
        const Vector3& operator[](int index) const { return vertices[index]; }
    };


    Vector3 Rotate(Vector3 vector, const Vector3& axis, const float theta)
    {
        float rotx = cosf(theta * 0.5f);
        float roty = sinf(theta * 0.5f);

        Vector3 result = vector;

        if (axis.x == 1)
        {
            result.y = vector.y *  rotx + vector.z * roty;
            result.z = vector.y * -roty + vector.z * rotx;
            vector = result;
        }
        if (axis.y == 1)
        {
            result.x = vector.x * rotx + vector.y * -roty;
            result.y = vector.x * roty + vector.y *  rotx;
            vector = result;
        }
        if (axis.z == 1)
        {
            result.x = vector.x * rotx + vector.y * roty;
            result.y = vector.x * -roty + vector.y * rotx;
            vector = result;
        }

        return result;
    }

    Vector3 PreRotate(Vector3 vector, const Vector3& axis, const float theta, const float rotx, const float roty)
    {
        Vector3 result = vector;

        if (axis.x == 1)
        {
            result.y = vector.y * rotx + vector.z * roty;
            result.z = vector.y * -roty + vector.z * rotx;
            vector = result;
        }
        if (axis.y == 1)
        {
            result.x = vector.x * rotx + vector.y * -roty;
            result.y = vector.x * roty + vector.y * rotx;
            vector = result;
        }
        if (axis.z == 1)
        {
            result.x = vector.x * rotx + vector.y * roty;
            result.y = vector.x * -roty + vector.y * rotx;
        }

        return result;
    }

    //how to lines intersect with planes
    Vector3 IntersectPlane(Vector3& plane_p, Vector3& plane_n, Vector3& lineStart, Vector3& lineEnd)
    {
        plane_n = plane_n.normalize();
        float plane_d = -plane_n.dot(plane_p);
        float ad = lineStart.dot(plane_n);
        float bd = lineEnd.dot(plane_n);
        float t = (-plane_d - ad) / (bd - ad);
        Vector3 lineStartToEnd = lineEnd - lineStart;
        Vector3 lineToIntersect = lineStartToEnd * t;
        return lineStart + lineToIntersect;
    }

    int ClipAgainstPlane(Vector3 plane_p, Vector3 plane_n, Triangle& in_tri, Triangle& out_tri1, Triangle& out_tri2)
    {
        // Make sure plane normal is indeed normal
        plane_n = plane_n.normalize();

        // Return signed shortest distance from point to plane, plane normal must be normalised
        auto dist = [&](Vector3& p)
            {
                Vector3 n = p.normalize();
                return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - plane_n.dot(plane_p));
            };

        // Create two temporary storage arrays to classify points either side of plane
        // If distance sign is positive, point lies on "inside" of plane
        Vector3* inside_points[3];  int nInsidePointCount = 0;
        Vector3* outside_points[3]; int nOutsidePointCount = 0;

        // Get signed distance of each point in triangle to plane
        float d0 = dist(in_tri[0]);
        float d1 = dist(in_tri[1]);
        float d2 = dist(in_tri[2]);

        if (d0 >= 0) { inside_points[nInsidePointCount++] = &in_tri[0]; }
        else { outside_points[nOutsidePointCount++] = &in_tri[0]; }
        if (d1 >= 0) { inside_points[nInsidePointCount++] = &in_tri[1]; }
        else { outside_points[nOutsidePointCount++] = &in_tri[1]; }
        if (d2 >= 0) { inside_points[nInsidePointCount++] = &in_tri[2]; }
        else { outside_points[nOutsidePointCount++] = &in_tri[2]; }

        // Now classify triangle points, and break the input triangle into 
        // smaller output triangles if required. There are four possible
        // outcomes...

        if (nInsidePointCount == 0)
        {
            // All points lie on the outside of plane, so clip whole triangle
            // It ceases to exist

            return 0; // No returned triangles are valid
        }

        if (nInsidePointCount == 3)
        {
            // All points lie on the inside of plane, so do nothing
            // and allow the triangle to simply pass through
            out_tri1 = in_tri;

            return 1; // Just the one returned original triangle is valid
        }

        if (nInsidePointCount == 1 && nOutsidePointCount == 2)
        {
            // Triangle should be clipped. As two points lie outside
            // the plane, the triangle simply becomes a smaller triangle

            // Copy appearance info to new triangle
            out_tri1.color = in_tri.color;

            // The inside point is valid, so keep that...
            out_tri1[0] = *inside_points[0];

            // but the two new points are at the locations where the 
            // original sides of the triangle (lines) intersect with the plane
            out_tri1[1] = IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);
            out_tri1[2] = IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[1]);

            return 1; // Return the newly formed single triangle
        }

        if (nInsidePointCount == 2 && nOutsidePointCount == 1)
        {
            // Triangle should be clipped. As two points lie inside the plane,
            // the clipped triangle becomes a "quad". Fortunately, we can
            // represent a quad with two new triangles

            // Copy appearance info to new triangles
            out_tri1.color = in_tri.color;

            out_tri2.color = in_tri.color;

            // The first triangle consists of the two inside points and a new
            // point determined by the location where one side of the triangle
            // intersects with the plane
            out_tri1[0] = *inside_points[0];
            out_tri1[1] = *inside_points[1];
            out_tri1[2] = IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);

            // The second triangle is composed of one of he inside points, a
            // new point determined by the intersection of the other side of the 
            // triangle and the plane, and the newly created point above
            out_tri2[0] = *inside_points[1];
            out_tri2[1] = out_tri1[2];
            out_tri2[2] = IntersectPlane(plane_p, plane_n, *inside_points[1], *outside_points[0]);

            return 2; // Return two newly formed triangles which form a quad
        }
    }

	void MultiplyVectorMatrix(Vector3& vector, const Matrix4x4& matrix)
	{
        Vector3 result;
        result.x = vector.x * matrix[0][0] + vector.y * matrix[1][0] + vector.z * matrix[2][0] + vector.w * matrix[3][0];
        result.y = vector.x * matrix[0][1] + vector.y * matrix[1][1] + vector.z * matrix[2][1] + vector.w * matrix[3][1];
        result.z = vector.x * matrix[0][2] + vector.y * matrix[1][2] + vector.z * matrix[2][2] + vector.w * matrix[3][2];
        result.w = vector.x * matrix[0][3] + vector.y * matrix[1][3] + vector.z * matrix[2][3] + vector.w * matrix[3][3];

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

