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
    meshCube.loadObjectFromFile("assets/axis.obj");

    float fNear = 0.1f;
    float fFar = 1000.0f;
    float fFov = 90.0f;
    float fAspectRatio = (float)window.getSize().x / (float)window.getSize().y;
    // float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

    matProj = makeProjection(fFov, fAspectRatio, fNear, fFar);

    return true;
}

bool Engine3D::onUserUpdate(float fElapsedTime) {
    // Up and down are swapped because of SFML window dimensions.
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        vCamera.y += 8.0f * fElapsedTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        vCamera.y -= 8.0f * fElapsedTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        vCamera.x -= 8.0f * fElapsedTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        vCamera.x += 8.0f * fElapsedTime;
    }

    Vec3D vForward = vLookDir * (8.0f * fElapsedTime);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        vCamera = vCamera + vForward;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        vCamera = vCamera - vForward;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        fYaw += 2.0f * fElapsedTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
        fYaw -= 2.0f * fElapsedTime;
    }

    window.clear(sf::Color::Black);
    // Set up rotation matrices
    mat4x4 matRotZ, matRotX, matRotY;
    // fTheta += 1.0f * fElapsedTime;

    // Rotation X
    matRotX = makeRotationX(fTheta);
    // Rotation Y
    matRotY = makeRotationY(fTheta);
    // Rotation Z
    matRotZ = makeRotationZ(fTheta);

    mat4x4 matTrans;
    matTrans = makeTranslation(0.0f, 0.0f, 10.0f);

    mat4x4 matWorld;
    matWorld = makeIdentity();
    matWorld = multiplyMatrix(matRotZ, matRotX);
    matWorld = multiplyMatrix(matWorld, matTrans);

    Vec3D vUp = { 0, 1, 0 };
    Vec3D vTarget = { 0, 0, 1 };
    mat4x4 matCameraRot = makeRotationY(fYaw);
    vLookDir = vTarget * matCameraRot;
    vTarget = vCamera + vLookDir;
    mat4x4 matCamera = pointAt(vCamera, vTarget, vUp);
    mat4x4 matView = quickInverse(matCamera);
    std::vector<Triangle> trianglesToRaster;

    // Draw triangles:
    for(auto tri : meshCube.tris) {
        Triangle triProjected, triTranslated, triViewed, triTransformed;

        triTransformed.p[0] = tri.p[0] * matWorld; // Matrix_MultiplyVector(matWorld, tri.p[0]);
        triTransformed.p[1] = tri.p[1] * matWorld; // Matrix_MultiplyVector(matWorld, tri.p[1]);
        triTransformed.p[2] = tri.p[2] * matWorld; // Matrix_MultiplyVector(matWorld, tri.p[2]);
        // Rotate X Axis:
        // tri = tri.rotate(matRotX);
        // Rotate Y Axis:
        // tri = tri.rotate(matRotY);
        // Rotate Z Axis:
        // tri = tri.rotate(matRotZ);
        // Offset (only to Z so we are not in the cube):
        triTranslated = triTransformed;  //.offset({0.0f, 0.0f, 5.0f});

        // Normals:
        Vec3D normal = triTranslated.calculateNormal();

        Vec3D vCameraRay = triTranslated.p[0] - vCamera;

        // triTranslated.isFacingCamera(normal, vCamera)
        if (normal.dotProduct(vCameraRay) < 0.0f) {
            Vec3D light_direction = { 0.0f, 0.0f, -1.0f };
            float l = sqrtf(light_direction.x * light_direction.x + light_direction.y * light_direction.y + light_direction.z * light_direction.z);
            light_direction.x /= l; light_direction.y /= l; light_direction.z /= l;

            float dp = std::max(0.1f, normal.x * light_direction.x + normal.y * light_direction.y + normal.z * light_direction.z);

            sf::Color shadedRed(sf::Uint8(255 * dp), sf::Uint8(0), sf::Uint8(0));
            
            // Tri viewed is bugged, find out why !!!
            triViewed.p[0] = triTranslated.p[0] * matView;
            triViewed.p[1] = triTranslated.p[1] * matView;
            triViewed.p[2] = triTranslated.p[2] * matView;

            // Clip Viewed Triangle against near plane, this could form two additional
            // additional triangles. 
            // int nClippedTriangles = 0;
            // Triangle clipped[2];
            // nClippedTriangles = Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, triViewed, clipped[0], clipped[1]);

            // We may end up with multiple triangles form the clip, so project as
            // required
            /*
            for (int n = 0; n < nClippedTriangles; n++)
            {
                // Project triangles from 3D --> 2D
                triProjected.p[0] = clipped[n].p[0] * matProj;  // Matrix_MultiplyVector(matProj, clipped[n].p[0]);
                triProjected.p[1] = clipped[n].p[1] * matProj;  // Matrix_MultiplyVector(matProj, clipped[n].p[1]);
                triProjected.p[2] = clipped[n].p[2] * matProj;  // Matrix_MultiplyVector(matProj, clipped[n].p[2]);
                triProjected.color = clipped[n].color;

                // Scale into view, we moved the normalising into cartesian space
                // out of the matrix.vector function from the previous videos, so
                // do this manually
                triProjected.p[0] = triProjected.p[0] / triProjected.p[0].w;  // Vector_Div(triProjected.p[0], triProjected.p[0].w);
                triProjected.p[1] = triProjected.p[1] / triProjected.p[1].w;  // Vector_Div(triProjected.p[1], triProjected.p[1].w);
                triProjected.p[2] = triProjected.p[2] / triProjected.p[2].w;  // Vector_Div(triProjected.p[2], triProjected.p[2].w);

                // X/Y are inverted so put them back
                triProjected.p[0].x *= -1.0f;
                triProjected.p[1].x *= -1.0f;
                triProjected.p[2].x *= -1.0f;
                triProjected.p[0].y *= -1.0f;
                triProjected.p[1].y *= -1.0f;
                triProjected.p[2].y *= -1.0f;

                // Offset verts into visible normalised space
                Vec3D vOffsetView = { 1,1,0 };
                triProjected.p[0] = triProjected.p[0] + vOffsetView;  // Vector_Add(triProjected.p[0], vOffsetView);
                triProjected.p[1] = triProjected.p[1] + vOffsetView;  // Vector_Add(triProjected.p[1], vOffsetView);
                triProjected.p[2] = triProjected.p[2] + vOffsetView;  // Vector_Add(triProjected.p[2], vOffsetView);
                triProjected.p[0].x *= 0.5f * 800;
                triProjected.p[0].y *= 0.5f * 800;
                triProjected.p[1].x *= 0.5f * 800;
                triProjected.p[1].y *= 0.5f * 800;
                triProjected.p[2].x *= 0.5f * 800;
                triProjected.p[2].y *= 0.5f * 800;

                // Store triangle for sorting
                triProjected.color = shadedRed;
                trianglesToRaster.push_back(triProjected);
            }
            */
            triProjected = triViewed.project(matProj);

            triProjected.p[0] = triProjected.p[0] / triProjected.p[0].w;
            triProjected.p[1] = triProjected.p[1] / triProjected.p[1].w;
            triProjected.p[2] = triProjected.p[2] / triProjected.p[2].w;

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
