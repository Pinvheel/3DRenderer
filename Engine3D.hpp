#pragma once

#include "structs.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>

class Engine3D {
 public:
    Engine3D();

    void run();

    bool onUserCreate();
    bool onUserUpdate(float fElapsedTime);

 private:
    Mesh meshCube;
    mat4x4 matProj;
    float fTheta;
    sf::RenderWindow window;

    void MultiplyMatrixVector(Vec3D &input, Vec3D &output, mat4x4 &matrix);
};