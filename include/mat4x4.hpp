#pragma once

#include "Vec3D.hpp"

struct mat4x4 { 
    float m[4][4] = { 0 };
};

mat4x4 makeIdentity();
mat4x4 makeRotationX(float fAngleRad);
mat4x4 makeRotationY(float fAngleRad);
mat4x4 makeRotationZ(float fAngleRad);
mat4x4 makeTranslation(float x, float y, float z);
mat4x4 makeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar);
mat4x4 multiplyMatrix(mat4x4 &m1, mat4x4 &m2);
mat4x4 pointAt(Vec3D &pos, Vec3D &target, Vec3D &up);
mat4x4 quickInverse(mat4x4 &m);