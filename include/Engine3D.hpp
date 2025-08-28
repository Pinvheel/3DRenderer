#pragma once

#include "Mesh.hpp"

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
    Vec3D vCamera = { 0, 0, 0 };
};