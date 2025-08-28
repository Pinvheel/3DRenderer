#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

struct mat4x4 { float m[4][4] = {}; };

struct Vec3D {  
    float x, y, z; 
};

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
    bool isFacingCamera(const Vec3D &normal);
};

struct Mesh { 
    std::vector<Triangle> tris; 

    bool loadObjectFromFile(std::string fileName);
};

