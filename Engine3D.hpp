#pragma once

#include "structs.hpp"
#include "math_utils.hpp"
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
    Vec3D vCamera = { 0 };
};