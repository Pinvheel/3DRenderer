#include "Vec3D.hpp"
#include "mat4x4.hpp"
#include <cmath>

float Vec3D::dotProduct(const Vec3D &vector) {
    return { x * vector.x + y * vector.y + z * vector.z };
}

float Vec3D::length() {
    return sqrt(dotProduct(*this));
}

Vec3D Vec3D::normalize() {
    float l  = length();
    return { x / l, y / l, z / l };
}

Vec3D Vec3D::crossProduct(const Vec3D &vector) {
    Vec3D cp;
    cp.x = y * vector.z - z * vector.y;
    cp.y = z * vector.x - x * vector.z;
    cp.z = x * vector.y - y * vector.x;
    return cp;
}

Vec3D operator+(const Vec3D &lhs, const Vec3D &rhs) {
    return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}

Vec3D operator-(const Vec3D &lhs, const Vec3D &rhs) {
    return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
}

Vec3D operator*(const Vec3D &vector, const float k) {
    return { vector.x * k, vector.y * k, vector.z * k };
}

Vec3D operator*(const Vec3D &vector, const mat4x4 &matrix) {
    Vec3D product;
    product.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + vector.w * matrix.m[3][0];
    product.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + vector.w * matrix.m[3][1];
    product.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + vector.w * matrix.m[3][2];
    product.w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + vector.w * matrix.m[3][3];
    return product;
}

Vec3D operator/(const Vec3D &vector, const float k) {
    return { vector.x / k, vector.y / k, vector.z / k };
}

Vec3D intersectPlane(Vec3D &plane_p, Vec3D &plane_n, Vec3D &lineStart, Vec3D &lineEnd) {
    plane_n.normalize();
    float plane_d = plane_n.dotProduct(plane_p);  // -Vector_DotProduct(plane_n, plane_p);
    float ad = lineStart.dotProduct(plane_n);  // Vector_DotProduct(lineStart, plane_n);
    float bd = lineEnd.dotProduct(plane_n);  // Vector_DotProduct(lineEnd, plane_n);
    float t = (-plane_d - ad) / (bd - ad);
    Vec3D lineStartToEnd = lineEnd - lineStart;  // Vector_Sub(lineEnd, lineStart);
    Vec3D lineToIntersect = lineStartToEnd * t;  // Vector_Mul(lineStartToEnd, t);
    return lineStart + lineToIntersect;  // Vector_Add(lineStart, lineToIntersect);
}