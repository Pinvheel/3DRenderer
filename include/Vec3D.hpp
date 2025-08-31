#pragma once

struct mat4x4;

struct Vec3D {  
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 1.0f;

    float dotProduct(const Vec3D &v);
    float length();
    Vec3D normalize();
    Vec3D crossProduct(const Vec3D &v);
};

Vec3D operator+(const Vec3D &lhs, const Vec3D &rhs);
Vec3D operator-(const Vec3D &lhs, const Vec3D &rhs);
Vec3D operator*(const Vec3D &lhs, const float k);
Vec3D operator*(const Vec3D &vector, const mat4x4 &matrix);
Vec3D operator/(const Vec3D &lhs, const float k);