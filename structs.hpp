#pragma once
#include <vector>

struct mat4x4 { float m[4][4] = {}; };

struct Vec3D {  
    float x, y, z; 

    Vec3D& operator+=(const float addend);
};

struct Triangle { 
    Vec3D p[3]; 

    Triangle rotate(const mat4x4 &matrix);

    Triangle offsetX(const float units);
    Triangle offsetY(const float units);
    Triangle offsetZ(const float units);

    Triangle project(const mat4x4 &matrix);
    void scale(float screenWidth, float screenHeight);
};

struct Mesh { std::vector<Triangle> tris; };

