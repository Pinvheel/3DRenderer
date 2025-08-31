#include "Triangle.hpp"
#include "math_utils.hpp"
#include <cmath>

// Rotates this triangle around the Z axis with the specified rotation matrix.
Triangle Triangle::rotate(const mat4x4 &matrix) {
    Triangle rotated;
    for (int i = 0; i < 3; i++) {
        MultiplyMatrixVector(p[i], rotated.p[i], matrix);
    }
    return rotated;
}

// Offsets the designated coordinate by the designated amount of units.
Triangle Triangle::offset(const Vec3D units) {
    Triangle offset;
    for (int i = 0; i < 3; i++) {
        offset.p[i].x = p[i].x + units.x;
        offset.p[i].y = p[i].y + units.y;
        offset.p[i].z = p[i].z + units.z;
    }
    return offset;
}

// Projects 3D triangle into a 2D 
Triangle Triangle::project(const mat4x4 &matrix) {
    Triangle projected;
    for (int i = 0; i < 3; i++) {
        MultiplyMatrixVector(p[i], projected.p[i], matrix);
    }
    return projected;
}

// Calculates this triangle's normal.
Vec3D Triangle::calculateNormal() {
        Vec3D normal, line1, line2;
        line1.x = p[1].x - p[0].x;
        line1.y = p[1].y - p[0].y;
        line1.z = p[1].z - p[0].z;

        line2.x = p[2].x - p[0].x;
        line2.y = p[2].y - p[0].y;
        line2.z = p[2].z - p[0].z;

        normal.x = line1.y * line2.z - line1.z * line2.y;
        normal.y = line1.z * line2.x - line1.x * line2.z;
        normal.z = line1.x * line2.y - line1.y * line2.x;

        float l = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
        normal.x /= l; normal.y /= l; normal.z /= l;
        return normal;
}

// Scales to the designated screenWidth and screenHeight (use literals for best results).
void Triangle::scale(float screenWidth, float screenHeight) {
    for (int i = 0; i < 3; i++) {
        p[i].x += 1;
        p[i].y += 1;

        p[i].x *= 0.5 * screenWidth;
        p[i].y *= 0.5 * screenHeight;
    }
}

// Determines if this normal is facing the camera.
bool Triangle::isFacingCamera(const Vec3D &normal, const Vec3D &cameraPos) {
    float xComp = normal.x * (p[0].x - cameraPos.x);
    float yComp = normal.y * (p[0].y - cameraPos.y);
    float zComp = normal.z * (p[0].z - cameraPos.z);
    bool isFacingCamera = xComp + yComp + zComp < 0.0f;
    return isFacingCamera;
}
 