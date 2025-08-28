#include "Engine3D.hpp"
#include <cmath>
#include <algorithm>

Engine3D::Engine3D() : window(sf::VideoMode(800, 800), "SFML Renderer") { onUserCreate(); }

void Engine3D::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        float fElapsedTime = clock.restart().asSeconds();
        onUserUpdate(fElapsedTime);
        window.display();
    }
}

bool Engine3D::onUserCreate() {
    // meshCube is composed of 12 triangles, with points defined here:
    meshCube.loadObjectFromFile("assets/VideoShip.obj");

    float fNear = 0.1f;
    float fFar = 1000.0f;
    float fFov = 90.0f;
    float fAspectRatio = (float)window.getSize().x / (float)window.getSize().y;
    float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

    matProj.m[0][0] = fAspectRatio * fFovRad;
    matProj.m[1][1] = fFovRad;
    matProj.m[2][2] = fFar / (fFar - fNear);
    matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    matProj.m[2][3] = 1.0f;
    matProj.m[3][3] = 0.0f;

    return true;
}

bool Engine3D::onUserUpdate(float fElapsedTime) {
    window.clear(sf::Color::Black);
    // Set up rotation matrices
    mat4x4 matRotZ, matRotX, matRotY;
    fTheta += 1.0f * fElapsedTime;

    // Rotation Z
    matRotZ.m[0][0] = cosf(fTheta);
    matRotZ.m[0][1] = sinf(fTheta);
    matRotZ.m[1][0] = -sinf(fTheta);
    matRotZ.m[1][1] = cosf(fTheta);
    matRotZ.m[2][2] = 1;
    matRotZ.m[3][3] = 1;

    // Rotation X
    matRotX.m[0][0] = 1;
    matRotX.m[1][1] = cosf(fTheta * 0.5f);
    matRotX.m[1][2] = sinf(fTheta * 0.5f);
    matRotX.m[2][1] = -sinf(fTheta * 0.5f);
    matRotX.m[2][2] = cosf(fTheta * 0.5f);
    matRotX.m[3][3] = 1;

    // Rotation Y
    matRotY.m[0][0] =  cosf(fTheta * 0.5f);
    matRotY.m[0][2] =  sinf(fTheta * 0.5f);
    matRotY.m[1][1] =  1.0f;
    matRotY.m[2][0] = -sinf(fTheta * 0.5f);
    matRotY.m[2][2] =  cosf(fTheta * 0.5f);
    matRotY.m[3][3] =  1.0f;

    std::vector<Triangle> trianglesToRaster;

    // Draw triangles:
    for(auto tri : meshCube.tris) {
        Triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;
        // Rotate Z Axis:
        triRotatedZ = tri.rotate(matRotZ);

        // Rotate X Axis:
        triRotatedZX = triRotatedZ.rotate(matRotX);

        // Offset (only to Z so we are not in the cube):
        triTranslated = triRotatedZX.offsetZ(8.0f);

        // Normals:
        Vec3D normal = triTranslated.calculateNormal();

        // Project triangles from 3D --> 2D
        if (triTranslated.isFacingCamera(normal, vCamera)) {
            Vec3D light_direction = { 0.0f, 0.0f, -1.0f };
            float l = sqrtf(light_direction.x * light_direction.x + light_direction.y * light_direction.y + light_direction.z * light_direction.z);
            light_direction.x /= l; light_direction.y /= l; light_direction.z /= l;

            float dp = std::max(0.0f, normal.x * light_direction.x + normal.y * light_direction.y + normal.z * light_direction.z);

            sf::Color shadedRed(sf::Uint8(255 * dp), sf::Uint8(0), sf::Uint8(0));
            
            
            triProjected = triTranslated.project(matProj);

            // Scale into view 
            triProjected.scale(800.0f, 800.0f);

            // Wireframes (Outdated, does not work with sort)
            /*
            sf::VertexArray edges(sf::Lines, 6);

            // edge 1
            edges[0].position = sf::Vector2f(triProjected.p[0].x, triProjected.p[0].y);
            edges[1].position = sf::Vector2f(triProjected.p[1].x, triProjected.p[1].y);

            // edge 2
            edges[2].position = sf::Vector2f(triProjected.p[1].x, triProjected.p[1].y);
            edges[3].position = sf::Vector2f(triProjected.p[2].x, triProjected.p[2].y);

            // edge 3
            edges[4].position = sf::Vector2f(triProjected.p[2].x, triProjected.p[2].y);
            edges[5].position = sf::Vector2f(triProjected.p[0].x, triProjected.p[0].y);

            // Set color for all
            for (int i = 0; i < 6; i++) edges[i].color = sf::Color::Red;

            window.draw(edges);
            */
            triProjected.color = shadedRed;
            trianglesToRaster.push_back(triProjected);
        }
    }

    std::sort(trianglesToRaster.begin(), trianglesToRaster.end(), [](Triangle &t1, Triangle &t2){
        float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
        float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
        return z1 > z2;
    });

    for (auto triProjected : trianglesToRaster) {
        sf::VertexArray triangle(sf::Triangles, 3);
        triangle[0].position = sf::Vector2f(triProjected.p[0].x, triProjected.p[0].y);
        triangle[1].position = sf::Vector2f(triProjected.p[1].x, triProjected.p[1].y);
        triangle[2].position = sf::Vector2f(triProjected.p[2].x, triProjected.p[2].y);
        triangle[0].color = triangle[1].color = triangle[2].color = triProjected.color; //shadedRed;
        window.draw(triangle);
    }
    return true;
}
