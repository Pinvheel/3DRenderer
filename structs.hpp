#pragma once
#include <vector>

struct Vec3D {  
    float x, y, z; 

    Vec3D& operator+=(const float addend);
};

struct Triangle { Vec3D p[3]; };

struct Mesh { std::vector<Triangle> tris; };

struct mat4x4 { float m[4][4] = {}; };