#include "Engine3D.hpp"

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
    meshCube.tris = {
        // SOUTH
        { -1.0f,-1.0f,-1.0f,   -1.0f, 1.0f,-1.0f,    1.0f, 1.0f,-1.0f },
        { -1.0f,-1.0f,-1.0f,    1.0f, 1.0f,-1.0f,    1.0f,-1.0f,-1.0f },

        // EAST
        { 1.0f,-1.0f,-1.0f,     1.0f, 1.0f,-1.0f,    1.0f, 1.0f, 1.0f },
        { 1.0f,-1.0f,-1.0f,     1.0f, 1.0f, 1.0f,    1.0f,-1.0f, 1.0f },

        // NORTH
        { 1.0f,-1.0f, 1.0f,     1.0f, 1.0f, 1.0f,   -1.0f, 1.0f, 1.0f },
        { 1.0f,-1.0f, 1.0f,    -1.0f, 1.0f, 1.0f,   -1.0f,-1.0f, 1.0f },

        // WEST
        { -1.0f,-1.0f, 1.0f,   -1.0f, 1.0f, 1.0f,   -1.0f, 1.0f,-1.0f },
        { -1.0f,-1.0f, 1.0f,   -1.0f, 1.0f,-1.0f,   -1.0f,-1.0f,-1.0f },

        // TOP
        { -1.0f, 1.0f,-1.0f,   -1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
        { -1.0f, 1.0f,-1.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f,-1.0f },

        // BOTTOM
        { 1.0f,-1.0f, 1.0f,    -1.0f,-1.0f, 1.0f,   -1.0f,-1.0f,-1.0f },
        { 1.0f,-1.0f, 1.0f,    -1.0f,-1.0f,-1.0f,    1.0f,-1.0f,-1.0f },
    };

    float fNear = 0.1f;
    float fFar = 1000.0f;
    float fFov = 90.0f;
    float fAspectRatio = 800 / 800;
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
    mat4x4 matRotZ, matRotX;
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

    Vec3D cubeCenter = {0.5f, 0.5f, 0.5f};
    // Draw triangles:
    for(auto tri : meshCube.tris) {
        Triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;
        // Rotate Z Axis:
        MultiplyMatrixVector(tri.p[0], triRotatedZ.p[0], matRotZ);
        MultiplyMatrixVector(tri.p[1], triRotatedZ.p[1], matRotZ);
        MultiplyMatrixVector(tri.p[2], triRotatedZ.p[2], matRotZ);

        // Rotate X Axis:
        MultiplyMatrixVector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
        MultiplyMatrixVector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
        MultiplyMatrixVector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);

        //Offset:
        triTranslated = triRotatedZX;
        triTranslated.p[0].z = triRotatedZX.p[0].z + 3.0f;
        triTranslated.p[1].z = triRotatedZX.p[1].z + 3.0f;
        triTranslated.p[2].z = triRotatedZX.p[2].z + 3.0f;

        // Project triangles from 3D --> 2D
        MultiplyMatrixVector(triTranslated.p[0], triProjected.p[0], matProj);
        MultiplyMatrixVector(triTranslated.p[1], triProjected.p[1], matProj);
        MultiplyMatrixVector(triTranslated.p[2], triProjected.p[2], matProj);

        // Scale into view 
        triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
        triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
        triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;
        triProjected.p[0].x *= 0.5f * 800; // (IMPORTANT, 800s here represent screen parameters! change later!)
        triProjected.p[0].y *= 0.5f * 800;
        triProjected.p[1].x *= 0.5f * 800;
        triProjected.p[1].y *= 0.5f * 800;
        triProjected.p[2].x *= 0.5f * 800;
        triProjected.p[2].y *= 0.5f * 800;

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
        for (int i = 0; i < 6; i++) edges[i].color = sf::Color::White;

        window.draw(edges);

    }
    return true;
}

void Engine3D::MultiplyMatrixVector (Vec3D &input, Vec3D &output, mat4x4 &matrix) {
    output.x = input.x * matrix.m[0][0] + input.y * matrix.m[1][0] + input.z * matrix.m[2][0] + matrix.m[3][0];
    output.y = input.x * matrix.m[0][1] + input.y * matrix.m[1][1] + input.z * matrix.m[2][1] + matrix.m[3][1];
    output.z = input.x * matrix.m[0][2] + input.y * matrix.m[1][2] + input.z * matrix.m[2][2] + matrix.m[3][2];
    float w = input.x * matrix.m[0][3] + input.y * matrix.m[1][3] + input.z * matrix.m[2][3] + matrix.m[3][3];

    if (w != 0.0f) {
        output.x /= w; output.y /= w; output.z /= w;
    }
}