#pragma once

#include "Vec3D.hpp"
#include "mat4x4.hpp"
#include <SFML/Graphics.hpp>

struct Triangle { 
    Vec3D p[3];
    sf::Color color;

    Triangle rotate(const mat4x4 &matrix);

    Triangle offsetX(const float units);
    Triangle offsetY(const float units);
    Triangle offsetZ(const float units);
    Triangle offset(const Vec3D units);

    Triangle project(const mat4x4 &matrix);
    Vec3D calculateNormal();
    void scale(float screenWidth, float screenHeight);
    bool isFacingCamera(const Vec3D &normal, const Vec3D &cameraPos);
};

int Triangle_ClipAgainstPlane(Vec3D plane_p, Vec3D plane_n, Triangle &in_tri, Triangle &out_tri1, Triangle &out_tri2);