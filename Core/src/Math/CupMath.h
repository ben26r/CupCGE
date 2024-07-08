#pragma once

#include <functional>
#include <iostream>
#include <cmath>
#include <array>

namespace Cup
{
    template<class T>
    struct Vector3 {
        T x = 0;
        T y = 0;
        T z = 0;
        T w = 1;

        Vector3() = default;
        Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
        Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z), w(other.w) {}

        inline static Vector3 Up()    { return Vector3( 0,  1,  0); }
        inline static Vector3 Down()  { return Vector3( 0, -1,  0); }
        inline static Vector3 Left()  { return Vector3(-1,  0,  0); }
        inline static Vector3 Right() { return Vector3( 1,  0,  0); }
        inline static Vector3 Far()   { return Vector3( 0,  0,  1); }
        inline static Vector3 Near()  { return Vector3( 0,  0, -1); }

        //Vector3 Rotate(const Vector3& axis, const float theta);
        //Vector3 PreRotate(const Vector3& axis, const float theta, const float rotx, const float roty);

        // Addition
        Vector3 operator+  (const T& scalar) const { return Vector3(x + scalar, y + scalar, z + scalar); }
        Vector3& operator+=(const T& scalar) { x += scalar; y += scalar; z += scalar; return *this; }
        Vector3 operator+  (const Vector3& other) const { return Vector3(x + other.x, y + other.y, z + other.z); }
        Vector3& operator+=(const Vector3& other) { x += other.x; y += other.y; z += other.z; return *this; }
        // Subtraction
        Vector3 operator-(const T& scalar) const { return Vector3(x - scalar, y - scalar, z - scalar); }
        Vector3& operator-=(const T& scalar) const { x -= scalar; y -= scalar; z -= scalar; return *this; }
        Vector3 operator-(const Vector3& other) const { return Vector3(x - other.x, y - other.y, z - other.z); }
        Vector3& operator-=(const Vector3& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
        // Multiplication
        Vector3 operator*(const T& scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }
        Vector3& operator*=(const T& scalar) const { x *= scalar; y *= scalar; z *= scalar; return *this; }
        Vector3 operator*(const Vector3& other) const { return Vector3(x * other.x, y * other.y, z * other.z); }
        Vector3& operator*=(const Vector3& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }
        // Division
        Vector3 operator/(const T& scalar) const { return Vector3(x / scalar, y / scalar, z / scalar); }
        Vector3& operator/=(const T& scalar) const { x /= scalar; y /= scalar; z /= scalar; return *this; }
        Vector3 operator/(const Vector3& other) const { return Vector3(x / other.x, y / other.y, z / other.z); }
        Vector3& operator/=(const Vector3& other) { x /= other.x; y /= other.y; z /= other.z; return *this; }
        // Dot product
        T dot(const Vector3& other) const { return x * other.x + y * other.y + z * other.z; }
        // Cross product
        Vector3 cross(const Vector3& other) const { return Vector3( y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x ); }
        // Magnitude
        T magnitude() const { return T(std::sqrt(x * x + y * y + z * z)); }
        // Normalize
        Vector3 normalize() const { float mag = magnitude(); return Vector3(x / mag, y / mag, z / mag); }
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

    template<class T>
    struct Matrix4x4
    {
        T matrix[4][4] = { 0 };

        T* operator[](int index) { return matrix[index]; }
        const T* operator[](int index) const { return matrix[index]; }

        static Matrix4x4 Identity();
        static Matrix4x4 Scale(T x, T y, T z);
        static Matrix4x4 Translation(T x, T y, T z);
        static Matrix4x4 Zero();
        static Matrix4x4 Projection(T fovDegrees, T aspectRatio, T cnear, T cfar);
        static Matrix4x4 Rotation(const Vector3<T>& axis, T angle);
        static Matrix4x4 PointAt(const Vector3<T>& position, const Vector3<T>& target, const Vector3<T>& up);

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
        Matrix4x4 operator*(T scalar) const {
            Matrix4x4 result;
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    result.matrix[i][j] = matrix[i][j] * scalar;
                }
            }
            return result;
        }

        Vector3<T> operator*(const Vector3<T>& other) const {
            Vector3<T> result;
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

    template <typename T>
    struct Triangle
    {
        std::array<Vector3<T>, 3> vertices;
        Vector4 color;

        void Do(const std::function<void(Vector3<T>&)>& func)
        {
            for (Vector3<T>& vertex : vertices)
                func(vertex);
        }

        void operator*=(const Matrix4x4<T>& other)
        {
            vertices[0] = other * vertices[0];
            vertices[1] = other * vertices[1];
            vertices[2] = other * vertices[2];
        }

        Triangle operator*(const Matrix4x4<T>& other) const
        {
            Triangle result;
            result[0] = other * vertices[0];
            result[1] = other * vertices[1];
            result[2] = other * vertices[2];
            return result;
        }

        Vector3<T>& operator[](int index) { return vertices[index]; }
        const Vector3<T>& operator[](int index) const { return vertices[index]; }
    };

    template <class T>
    class Mesh
    {
    public:
        std::vector<Triangle<T>> triangles;

        bool LoadModel(const std::string& filepath);
    };

    //how to lines intersect with planes
    template<class T>
    Vector3<T> IntersectPlane(Vector3<T>& plane_p, Vector3<T>& plane_n, Vector3<T>& lineStart, Vector3<T>& lineEnd);

    template<class T>
    int ClipAgainstPlane(Vector3<T> plane_p, Vector3<T> plane_n, Triangle<T>& in_tri, Triangle<T>& out_tri1, Triangle<T>& out_tri2);

    template<class T>
    void MultiplyVectorMatrix(Vector3<T>& vector, const Matrix4x4<T>& matrix);

    template<class T>
    Vector3<T> MultiplyVectorMatrix(const Vector3<T>& vector, const Matrix4x4<T>& matrix);

    #pragma region Vector Rotation

    //template<class T>
    //Vector3<T> Vector3<T>::Rotate(const Vector3<T>& axis, const float theta)
    //{
    //    float rotx = cosf(theta * 0.5f);
    //    float roty = sinf(theta * 0.5f);

    //    Vector3 current = { x, y, z };
    //    Vector3 result;

    //    if (axis.x == 1)
    //    {
    //        result.y = current.y * rotx + current.z * roty;
    //        result.z = current.y * -roty + current.z * rotx;
    //        current = result;
    //    }
    //    if (axis.y == 1)
    //    {
    //        result.x = current.x * rotx + current.y * -roty;
    //        result.y = current.x * roty + current.y * rotx;
    //        current = result;
    //    }
    //    if (axis.z == 1)
    //    {
    //        result.x = current.x * rotx + current.y * roty;
    //        result.y = current.x * -roty + current.y * rotx;
    //        current = result;
    //    }

    //    return result;
    //}

    //template<class T>
    //Vector3<T> Vector3<T>::PreRotate(const Vector3<T>& axis, const float theta, const float rotx, const float roty)
    //{
    //    Vector3 current = { x, y, z };
    //    Vector3 result;

    //    if (axis.x == 1)
    //    {
    //        result.y = current.y * rotx + current.z * roty;
    //        result.z = current.y * -roty + current.z * rotx;
    //        current = result;
    //    }
    //    if (axis.y == 1)
    //    {
    //        result.x = current.x * rotx + current.y * -roty;
    //        result.y = current.x * roty + current.y * rotx;
    //        current = result;
    //    }
    //    if (axis.z == 1)
    //    {
    //        result.x = current.x * rotx + current.y * roty;
    //        result.y = current.x * -roty + current.y * rotx;
    //        current = result;
    //    }

    //    return result;
    //}
#pragma endregion
 
    template <class T>
    bool Mesh<T>::LoadModel(const std::string& filepath)
    {
        std::ifstream file(filepath);
        if (!file.is_open())
            return false;

        std::vector<Vector3<T>> vertices;
        std::string line;
        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;

            if (prefix == "v")
            {
                Vector3<T> vertex;
                iss >> vertex.x >> vertex.y >> vertex.z;
                vertices.push_back(vertex);
            }
            else if (prefix == "f")
            {
                Triangle<T> tri;
                int vertexIndices[3];
                for (int i = 0; i < 3; ++i)
                {
                    iss >> vertexIndices[i];
                    // OBJ files are 1-indexed
                    tri[i] = vertices[vertexIndices[i] - 1];
                }
                triangles.push_back(tri);
            }
        }

        file.close();
        return true;
    }

    template<class T>
    Matrix4x4<T> Matrix4x4<T>::Identity()
    {
        Matrix4x4 result;
        for (int i = 0; i < 4; ++i) {
            result.matrix[i][i] = 1.0f;
        }
        return result;
    }

    template<class T>
    Matrix4x4<T> Matrix4x4<T>::Scale(T x, T y, T z)
    {
        Matrix4x4 result;
        result[0][0] = x;
        result[1][1] = y;
        result[2][2] = z;
        result[3][3] = 1.0f;
        return result;
    }

    template<class T>
    Matrix4x4<T> Matrix4x4<T>::Translation(T x, T y, T z)
    {
        Matrix4x4 result;
        result[0][0] = 1.0f;
        result[1][1] = 1.0f;
        result[2][2] = 1.0f;
        result[3][3] = 1.0f;
        result[3][0] = x;
        result[3][1] = y;
        result[3][2] = z;
        return result;
    }

    // Static function to create a zero matrix
    template<class T>
    Matrix4x4<T> Matrix4x4<T>::Zero() {
        return Matrix4x4();
    }

    template<class T>
    Matrix4x4<T> Matrix4x4<T>::Projection(T fovDegrees, T aspectRatio, T cnear, T cfar)
    {
        T fFovRad = 1.0f / tanf(fovDegrees * 0.5f / 180.0f * 3.14159f);
        Matrix4x4 result;
        result[0][0] = aspectRatio * fFovRad;
        result[1][1] = fFovRad;
        result[2][2] = cfar / (cfar - cnear);
        result[3][2] = (-cfar * cnear) / (cfar - cnear);
        result[2][3] = 1.0f;
        result[3][3] = 0.0f;
        return result;
    }

    template<class T>
    Matrix4x4<T> Matrix4x4<T>::Rotation(const Vector3<T>& axis, T angle)
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

    template<class T>
    Matrix4x4<T> Matrix4x4<T>::PointAt(const Vector3<T>& position, const Vector3<T>& target, const Vector3<T>& up)
    {
        Vector3<T> newForward = target - position;
        newForward = newForward.normalize();

        Vector3<T> a = newForward * up.dot(newForward);
        Vector3<T> newUp = (up - a).normalize();

        Vector3<T> newRight = newUp.cross(newForward);

        Matrix4x4 result;
        result[0][0] = newRight.x;	    result[0][1] = newRight.y;	    result[0][2] = newRight.z;	    result[0][3] = 0.0f;
        result[1][0] = newUp.x;		    result[1][1] = newUp.y;		    result[1][2] = newUp.z;		    result[1][3] = 0.0f;
        result[2][0] = newForward.x;	result[2][1] = newForward.y;	result[2][2] = newForward.z;	result[2][3] = 0.0f;
        result[3][0] = position.x;		result[3][1] = position.y;		result[3][2] = position.z;		result[3][3] = 1.0f;
        return result;
    }

    template<class T>
    Matrix4x4<T> Matrix4x4<T>::QuickInverse() // Only for Rotation/Translation Matrices
    {
        Matrix4x4 result;
        result[0][0] = matrix[0][0];  result[0][1] = matrix[1][0];  result[0][2] = matrix[2][0];  result[0][3] = 0.0f;
        result[1][0] = matrix[0][1];  result[1][1] = matrix[1][1];  result[1][2] = matrix[2][1];  result[1][3] = 0.0f;
        result[2][0] = matrix[0][2];  result[2][1] = matrix[1][2];  result[2][2] = matrix[2][2];  result[2][3] = 0.0f;
        result[3][0] = -(matrix[3][0] * result[0][0] + matrix[3][1] * result[1][0] + matrix[3][2] * result[2][0]);
        result[3][1] = -(matrix[3][0] * result[0][1] + matrix[3][1] * result[1][1] + matrix[3][2] * result[2][1]);
        result[3][2] = -(matrix[3][0] * result[0][2] + matrix[3][1] * result[1][2] + matrix[3][2] * result[2][2]);
        result[3][3] = 1.0f;
        return result;
    }

    //how to lines intersect with planes
    template<class T>
    Vector3<T> IntersectPlane(Vector3<T>& plane_p, Vector3<T>& plane_n, Vector3<T>& lineStart, Vector3<T>& lineEnd)
    {
        plane_n = plane_n.normalize();
        float plane_d = -plane_n.dot(plane_p);
        float ad = lineStart.dot(plane_n);
        float bd = lineEnd.dot(plane_n);
        float t = (-plane_d - ad) / (bd - ad);
        Vector3<T> lineStartToEnd = lineEnd - lineStart;
        Vector3<T> lineToIntersect = lineStartToEnd * t;
        return lineStart + lineToIntersect;
    }

    template<class T>
    int ClipAgainstPlane(Vector3<T> plane_p, Vector3<T> plane_n, Triangle<T>& in_tri, Triangle<T>& out_tri1, Triangle<T>& out_tri2)
    {
        // Make sure plane normal is indeed normal
        plane_n = plane_n.normalize();

        // Return signed shortest distance from point to plane, plane normal must be normalised
        auto dist = [&](Vector3<T>& p)
            {
                Vector3<T> n = p.normalize();
                return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - plane_n.dot(plane_p));
            };

        // Create two temporary storage arrays to classify points either side of plane
        // If distance sign is positive, point lies on "inside" of plane
        Vector3<T>* inside_points[3];  int nInsidePointCount = 0;
        Vector3<T>* outside_points[3]; int nOutsidePointCount = 0;

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
            // Triangle<T> should be clipped. As two points lie outside
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
            // Triangle<T> should be clipped. As two points lie inside the plane,
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

    template<class T>
    void MultiplyVectorMatrix(Vector3<T>& vector, const Matrix4x4<T>& matrix)
    {
        Vector3<T> result;
        result.x = vector.x * matrix[0][0] + vector.y * matrix[1][0] + vector.z * matrix[2][0] + vector.w * matrix[3][0];
        result.y = vector.x * matrix[0][1] + vector.y * matrix[1][1] + vector.z * matrix[2][1] + vector.w * matrix[3][1];
        result.z = vector.x * matrix[0][2] + vector.y * matrix[1][2] + vector.z * matrix[2][2] + vector.w * matrix[3][2];
        result.w = vector.x * matrix[0][3] + vector.y * matrix[1][3] + vector.z * matrix[2][3] + vector.w * matrix[3][3];

        vector = result;
    }

    template<class T>
    Vector3<T> MultiplyVectorMatrix(const Vector3<T>& vector, const Matrix4x4<T>& matrix)
    {
        Vector3<T> result;
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

    //using Matrix4x4f = Matrix4x4<float>;
}

